using System;
using System.CodeDom.Compiler;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp;
using Microsoft.CodeAnalysis.Emit;
using Microsoft.CodeAnalysis.Text;

namespace ProjectModel
{
    internal class ModuleCompiler
    {
        public static bool CompileModule(string ModuleFilePath, string ModuleAssemblyFilePath)
        {
            CSharpParseOptions ParseOptions = new CSharpParseOptions(LanguageVersion.Latest, DocumentationMode.Parse, SourceCodeKind.Regular);
            SourceText Source = SourceText.From(File.ReadAllText(ModuleFilePath), System.Text.Encoding.UTF8);
            SyntaxTree Tree = CSharpSyntaxTree.ParseText(Source, ParseOptions, ModuleFilePath);
            IEnumerable<Diagnostic> Diagnostics = Tree.GetDiagnostics();
            if (Diagnostics.Any())
                return false;

            List<SyntaxTree> SyntaxTrees = new List<SyntaxTree>();
            SyntaxTrees.Add(Tree);
            List<MetadataReference> MetadataReferences = new List<MetadataReference>();
            MetadataReferences.Add(MetadataReference.CreateFromFile(typeof(object).Assembly.Location));
            MetadataReferences.Add(MetadataReference.CreateFromFile(Assembly.Load("System.Runtime").Location));
            MetadataReferences.Add(MetadataReference.CreateFromFile(Assembly.Load("System.Collections").Location));
            MetadataReferences.Add(MetadataReference.CreateFromFile(Assembly.Load("System.IO").Location));
            MetadataReferences.Add(MetadataReference.CreateFromFile(Assembly.Load("System.IO.FileSystem").Location));
            MetadataReferences.Add(MetadataReference.CreateFromFile(Assembly.Load("System.Linq").Location));
            MetadataReferences.Add(MetadataReference.CreateFromFile(Assembly.Load("System.Private.Xml").Location));
            MetadataReferences.Add(MetadataReference.CreateFromFile(Assembly.Load("System.Private.Xml.Linq").Location));
            MetadataReferences.Add(MetadataReference.CreateFromFile(Assembly.Load("System.Text.RegularExpressions").Location));
            MetadataReferences.Add(MetadataReference.CreateFromFile(Assembly.Load("System.Console").Location));
            //MetadataReferences.Add(MetadataReference.CreateFromFile(Assembly.Load("ProjectModel.dll").Location));

            string ProjectModelFilePath = Path.GetDirectoryName(Process.GetCurrentProcess().MainModule!.FileName)!;
            string ProjectModelAssemblyPath = Path.Combine(ProjectModelFilePath, "ProjectModel.dll");
            MetadataReferences.Add(MetadataReference.CreateFromFile(ProjectModelAssemblyPath));

            CSharpCompilationOptions CompilationOptions = new CSharpCompilationOptions(
                outputKind: OutputKind.DynamicallyLinkedLibrary,
#if DEBUG
                optimizationLevel: OptimizationLevel.Debug,
#else
				// Optimize the managed code in Development
				optimizationLevel: OptimizationLevel.Release,
#endif
                assemblyIdentityComparer: DesktopAssemblyIdentityComparer.Default,
                reportSuppressedDiagnostics: true
            );

            string AssemblyFileName = Path.GetFileName(ModuleAssemblyFilePath);
            string PdbFilePath = Path.ChangeExtension(ModuleAssemblyFilePath, ".pdb");

            EmitResult Result;
            CSharpCompilation Compilation = CSharpCompilation.Create(AssemblyFileName, SyntaxTrees, MetadataReferences, CompilationOptions);

            using (FileStream AssemblyFileStream = File.Open(ModuleAssemblyFilePath, FileMode.Create))
            using (FileStream PdbFileStream = File.Open(PdbFilePath, FileMode.Create))
            {
                EmitOptions EmitOptions = new EmitOptions(includePrivateMembers: true);
                Result = Compilation.Emit(
                    peStream: AssemblyFileStream,
                    pdbStream: PdbFileStream,
                    options: EmitOptions);
            }
            //LogDiagnostics(Result.Diagnostics);

            if (!Result.Success)
            {
                if (File.Exists(ModuleAssemblyFilePath))
                    File.Delete(ModuleAssemblyFilePath);
                if (File.Exists(PdbFilePath))
                    File.Delete(PdbFilePath);

                foreach (Diagnostic? Diagnostic in Result.Diagnostics)
                    Console.WriteLine($"    [{AssemblyFileName}] {Diagnostic}");
            }
            return Result.Success;
        }

        public static IModule? CompileModuleFromModuleFile(string ProjectRoot, string ModuleFilePath)
        {
            string ModuleAssemblyDiretory = Path.Combine(ProjectRoot, "Documents/Build/ProjectModules");
            if (!Directory.Exists(ModuleAssemblyDiretory))
                Directory.CreateDirectory(ModuleAssemblyDiretory);

            string ModuleName;
            if (Path.GetFileName(ModuleFilePath).Equals("Module.cs"))
                ModuleName = Path.GetFileName(Path.GetDirectoryName(ModuleFilePath))!;
            else
                ModuleName = Path.GetFileNameWithoutExtension(Path.GetFileNameWithoutExtension(ModuleFilePath));
            string ModuleAssemblyPath = Path.Combine(ModuleAssemblyDiretory, ModuleName + ".Module.dll");

            Assembly? EntryAssembly = Assembly.GetEntryAssembly();
            if (EntryAssembly == null)
                return null;

            bool NeedCompile = false;
            if (!File.Exists(ModuleAssemblyPath))
                NeedCompile = true;
            else
            {
                DateTime CompilerFileTime = new FileInfo(EntryAssembly.Location).LastWriteTime;
                DateTime ModuleAssemblyFileTime = new FileInfo(ModuleAssemblyPath).LastWriteTime;
                DateTime ModuleFileTime = new FileInfo(ModuleFilePath).LastWriteTime;

                NeedCompile = ModuleFileTime > ModuleAssemblyFileTime || CompilerFileTime > ModuleAssemblyFileTime;
            }

            //if (!NeedCompile)
            //    NeedCompile = ModuleName == "Xin.Core";

            if (NeedCompile)
            {
                if (!CompileModule(ModuleFilePath, ModuleAssemblyPath))
                {
                    Console.WriteLine($"Compile module \"{ModuleFilePath}\" failed!");
                    return null;
                }
                Console.WriteLine($"Compile module \"{ModuleFilePath}\" success.");
            }

            Assembly ModuleAssembly = Assembly.LoadFile(ModuleAssemblyPath);
            foreach (Type CompiledType in ModuleAssembly.GetTypes())
            {
                if (CompiledType.BaseType == typeof(IModule))
                {
                    ConstructorInfo? Constructor = CompiledType.GetConstructor(new Type[] { });
                    if (Constructor != null)
                    {
                        IModule? Module =  Constructor.Invoke(null) as IModule;
                        if (Module == null || Module.Type == ModuleType.None)
                            continue;

                        Module.Name = ModuleName;
                        Module.ModuleFilePath = ModuleFilePath;
                        return Module;
                    }
                }
            }

            return null;
        }
    }
}
