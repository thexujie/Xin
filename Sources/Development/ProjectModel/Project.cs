using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using System.Reflection.Metadata;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;
using System.Xml;
using Microsoft.CodeAnalysis.CSharp.Syntax;

namespace ProjectModel
{
    [DebuggerDisplay("{FullPath}, Children={Children}, Modules={Modules}")]
    public class ProjectVirtualFolder
    {
        public string Name { get; set; }

        public ProjectVirtualFolder? Parent { get; set; }
        public List<ProjectVirtualFolder> Children{ get; set; } = new();

        public string FullPath => Parent == null ? string.Empty : Path.Combine(Parent.FullPath, Name);

        public List<IProjectModule> Modules = new();

        public bool IsRoot => Parent == null;

        public ProjectVirtualFolder(ProjectVirtualFolder? Parent, string Name)
        {
            this.Parent = Parent;
            this.Name = Name;
        }
    }

    [DebuggerDisplay("{FileType} {FilePath}")]
    public class ProjectModuleFile
    {
        public const string FileType_H = "h";
        public const string FileType_HPP = "hpp";
        public const string FileType_C = "c";
        public const string FileType_CC = "cc";
        public const string FileType_CPP = "cpp";

        public string FileType { get; set; } = string.Empty;
        public string FilePath { get; set; } = string.Empty;
        public string? SpecifiedFilter { get; set; } = null;

        public string Filter
        {
            get
            {
                if (!string.IsNullOrEmpty(SpecifiedFilter))
                    return SpecifiedFilter;

                if (FilePath.StartsWith('.'))
                    return string.Empty;

                return Path.GetDirectoryName(FilePath) ?? string.Empty;
            }
        }
    }

    //[DebuggerDisplay("{Name} {Category} {Type} Module, {VirtualFolder}, {FullPath}")]
    public class IProjectModule
    {
        public Project Project { get; }
        public string VirtualFolder { get; }

        public ModuleCategory Category { get; }

        /// <summary>
        /// .vcxproj .cxproj path.
        /// </summary>
        public string FullPath { get; }

        public string ModuleName => Path.GetFileNameWithoutExtension(FullPath);
        public string ModuleRoot => Path.GetDirectoryName(FullPath)!;

        public IProjectModule(Project Project, string VirtualFolder, ModuleCategory Category, string FullPath)
        {
            this.Project = Project;
            this.VirtualFolder = VirtualFolder;
            this.Category = Category;
            this.FullPath = FullPath;
        }
        
        public string NormalizePath(string RelativePath)
        {
            if (Path.IsPathRooted(RelativePath))
                return RelativePath;

            foreach (string WorkingRoot in Project.WorkingRoots)
            {
                string FoundPath = Path.Combine(WorkingRoot, "Sources", RelativePath);
                if (Directory.Exists(FoundPath) || File.Exists(FoundPath))
                {
                    return Path.GetRelativePath(ModuleRoot, FoundPath);
                }
            }
            //Path = Path.Replace("$(Externals)", ExternalsDirectory);
            //Path = Path.Replace("$(Depends)", DependsDirectory);
            //Path = Path.Replace("$(Runtime)", RuntimeDirectory);
            return RelativePath;
        }
        
        
        public string NormalizePath(ModuleCondition Condition, string RelativePath)
        {
            string NormalizedPath = NormalizePath(RelativePath);
            if (NormalizedPath.Contains("$(Configuration)"))
                NormalizedPath = NormalizedPath.Replace("$(Configuration)", Condition.Configuration.ToString());
            if (NormalizedPath.Contains("$(Platform)"))
                NormalizedPath = NormalizedPath.Replace("$(Platform)", Condition.Platform.ToString());
            return NormalizedPath;
        }
    }

    [DebuggerDisplay("{Configuration}_{Platform}")]
    public record struct ModuleCondition
    {
        public ModuleConfiguration Configuration = ModuleConfiguration.Default;
        public ModulePlatform Platform = ModulePlatform.Default;

        public ModuleCondition()
        {

        }

        public ModuleCondition(ModuleConfiguration Configuration, ModulePlatform Platform)
        {
            this.Configuration = Configuration;
            this.Platform = Platform;
        }

        public static readonly ModuleCondition Default = new ModuleCondition() {Configuration = ModuleConfiguration.Default, Platform = ModulePlatform.Default };
        public static readonly ModuleCondition Debug = new ModuleCondition() {Configuration = ModuleConfiguration.Debug, Platform = ModulePlatform.Default };
        public static readonly ModuleCondition Release = new ModuleCondition() {Configuration = ModuleConfiguration.Release, Platform = ModulePlatform.Default };
        public static readonly ModuleCondition x86 = new ModuleCondition() { Configuration = ModuleConfiguration.Default, Platform = ModulePlatform.x86 };
        public static readonly ModuleCondition x64 = new ModuleCondition() { Configuration = ModuleConfiguration.Default, Platform = ModulePlatform.x64 };
        public static readonly ModuleCondition Debug_x86 = new ModuleCondition() {Configuration = ModuleConfiguration.Debug, Platform = ModulePlatform.x86};
        public static readonly ModuleCondition Debug_x64 = new ModuleCondition() {Configuration = ModuleConfiguration.Debug, Platform = ModulePlatform.x64};
        public static readonly ModuleCondition Release_x86 = new ModuleCondition() { Configuration = ModuleConfiguration.Release, Platform = ModulePlatform.x86 };
        public static readonly ModuleCondition Release_x64 = new ModuleCondition() { Configuration = ModuleConfiguration.Release, Platform = ModulePlatform.x64 };
    }

    [DebuggerDisplay("{VirtualFolder}, {Name} {Category} {Type} Module, {FullPath}")]
    public class CPPModule : IProjectModule
    {
        public IModule ModuleInterface { get; }

        public string Name => ModuleInterface.Name;
        public string LastName { get; }

        public ModuleType Type => ModuleInterface.Type;
        public ModuleSubType SubType => ModuleInterface.SubType;

        public string PCHFile => ModuleInterface.PCHFile;

        public List<ProjectModuleFile> Files = new();

        public List<CPPModule> PrivateDependencyModules = new();
        public List<CPPModule> PublicDependencyModules = new();

        public List<string> PrivateDefines = new();
        public List<string> PublicDefines = new();

        public HashSet<CPPModule> DependencyModules = new();

        public Dictionary<ModuleCondition, ModuleConditionalRules> ConditionalRules = new()
        {
            { ModuleCondition.Default, new ModuleConditionalRules() },
            { ModuleCondition.Debug, new ModuleConditionalRules() },
            { ModuleCondition.Release, new ModuleConditionalRules() },
            { ModuleCondition.Debug_x86, new ModuleConditionalRules() },
            { ModuleCondition.Debug_x64, new ModuleConditionalRules() },
            { ModuleCondition.Release_x86, new ModuleConditionalRules() },
            { ModuleCondition.Release_x64, new ModuleConditionalRules() },
        };

        public List<string> PrivateIncludePaths => ConditionalRules[ModuleCondition.Default].PrivateIncludePaths;

        public IEnumerable<string> ConditionalPrivateIncludePaths(ModuleCondition Condition)
        {
            foreach (string PrivateIncludePath in PrivateIncludePaths)
                yield return PrivateIncludePath;

            if (Condition != ModuleCondition.Default)
            {
                foreach (string PrivateIncludePath in ConditionalRules[Condition].PrivateIncludePaths)
                    yield return PrivateIncludePath;
            }
        }

        public List<string> PrivateLibraryPaths => ConditionalRules[ModuleCondition.Default].PrivateLibraryPaths;
        public List<string> PrivateLibraries => ConditionalRules[ModuleCondition.Default].PrivateLibraries;

        public IEnumerable<string> ConditionalPrivateLibraryPaths(ModuleCondition Condition)
        {
            foreach (string PrivateLibraryPath in PrivateLibraryPaths)
                yield return PrivateLibraryPath;

            if (Condition != ModuleCondition.Default)
            {
                foreach (string PrivateLibraryPath in ConditionalRules[Condition].PrivateLibraryPaths)
                    yield return PrivateLibraryPath;
            }
        }

        public IEnumerable<string> ConditionalPrivateLibrarys(ModuleCondition Condition)
        {
            foreach (string PrivateLibrarie in PrivateLibraries)
                yield return PrivateLibrarie;

            if (Condition != ModuleCondition.Default)
            {
                foreach (string PrivateLibrarie in ConditionalRules[Condition].PrivateLibraries)
                    yield return PrivateLibrarie;
            }
        }


        /// <summary>
        /// Full path for .vcxproj file
        /// </summary>
        public string VcxprojFilePath => Path.Combine(ModuleRoot, Name + ".vcxproj");

        private const string AdditionalFilesFilterName = "AdditionalFiles";

        public CPPModule(Project Project, string VirtualFolder, IModule ModuleInterface) 
            : base(Project, VirtualFolder, ModuleCategory.CPP, Path.Combine(Path.GetDirectoryName(ModuleInterface.ModuleFilePath)!, ModuleInterface.Name + ".vcxproj"))
        {
            this.ModuleInterface = ModuleInterface;
            this.LastName = ModuleInterface.Name.Split('.').Last();

            PrivateDefines.AddRange(ModuleInterface.PrivateDefines);
            PublicDefines.AddRange(ModuleInterface.PublicDefines);

            UpdateIncludePaths();
            UpdateLibraries();
            UpdateFiles();
        }

        private void AppendFile(string FilePath, string? SpecifiedFilter = null)
        {
            if (!Path.IsPathRooted(FilePath))
                FilePath = Path.GetFullPath(FilePath, ModuleRoot);

            string Extention = Path.GetExtension(FilePath);
            if (Extention.Length < 2)
                return;

            Extention = Extention.Substring(1).ToLower();
            switch (Extention)
            {
            case "h":
            case "hpp":
            case "cpp":
            case "cxx":
            case "c":
            case "cc":
            case "inl":
            case "cs":
            case "props":
                break;
            case "natvis":
                break;
            case "xsh":
            case "xsf":
                break;
            case "bmp":
            case "jpg":
            case "jpeg":
            case "png":
            case "tga":
            case "dds":
                break;
            default:
                return;
            }

            string RelatvieFilePath = Path.GetRelativePath(ModuleRoot, FilePath);
            if (RelatvieFilePath.StartsWith('.'))
            {
                ProjectModuleFile? FoundModuleFile = Files.Find(X => X.FilePath.Equals(FilePath));
                if (FoundModuleFile == null)
                    Files.Add(new ProjectModuleFile() { FileType = Extention, FilePath = FilePath, SpecifiedFilter = SpecifiedFilter });
            }
            else
            {
                ProjectModuleFile? FoundModuleFile = Files.Find(X => X.FilePath.Equals(RelatvieFilePath));
                if (FoundModuleFile == null)
                    Files.Add(new ProjectModuleFile() { FileType = Extention, FilePath = RelatvieFilePath, SpecifiedFilter = SpecifiedFilter });
            }
        }

        private void SearchFiles(string SearchDirectory, string SearchPattern, string? SpecifiedFilter = null)
        {
            if (!Directory.Exists(SearchDirectory))
                return;

            if (new DirectoryInfo(SearchDirectory).Name.StartsWith("__"))
                return;

            foreach (string FilePath in Directory.EnumerateFiles(SearchDirectory, SearchPattern))
                AppendFile(FilePath, SpecifiedFilter);
        }

        private void UpdateFiles()
        {
            if (ModuleInterface.Files.Count == 0)
            {
                SearchFiles(ModuleRoot, "*");
                foreach (string ProjectFolders in Directory.EnumerateDirectories(ModuleRoot, "*", SearchOption.AllDirectories))
                    SearchFiles(ProjectFolders, "*");
            }
            else
            {
                foreach (string FilePattern in ModuleInterface.Files)
                {
                    if (FilePattern.Contains('*'))
                    {
                        SearchFiles(ModuleRoot, FilePattern);
                    }
                    else
                    {
                        AppendFile(FilePattern);
                    }
                }
            }

            foreach (string FilePattern in ModuleInterface.AdditionalFiles)
            {
                if (FilePattern.Contains('*'))
                {
                    SearchFiles(ModuleRoot, FilePattern, AdditionalFilesFilterName);
                }
                else
                {
                    AppendFile(FilePattern, AdditionalFilesFilterName);
                }
            }

            AppendFile(ModuleInterface.ModuleFilePath);
            Files.Sort((A, B) => A.FilePath.CompareTo(B.FilePath));
        }

        private void UpdateIncludePaths()
        {
            List<string> ConditionalIncludePaths = new List<string>();
            foreach (string PrivateIncludePath in ModuleInterface.PrivateIncludePaths)
            {
                if (PrivateIncludePath.Contains("$(Configuration)") || PrivateIncludePath.Contains("$(Platform)"))
                {
                    ConditionalIncludePaths.Add(PrivateIncludePath);
                    continue;
                }
                PrivateIncludePaths.Add(NormalizePath(PrivateIncludePath));
            }

            foreach (ModuleCondition Condition in Project.Conditions)
            {
                foreach (string ConditionalIncludePath in ConditionalIncludePaths)
                {
                    string IncludePath = ConditionalIncludePath.Replace("$(Configuration)", Condition.Configuration.ToString());
                    IncludePath = IncludePath.Replace("$(Platform)", Condition.Platform.ToString());
                    ConditionalRules[Condition].PrivateIncludePaths.Add(IncludePath);
                }

                foreach (string PrivateIncludePath in ModuleInterface.ConditionalRules[ModuleCondition.Default].PrivateIncludePaths)
                {
                    ConditionalRules[Condition].PrivateIncludePaths.Add(NormalizePath(PrivateIncludePath));
                }
                foreach (string PrivateIncludePath in ModuleInterface.ConditionalRules[Condition].PrivateIncludePaths)
                {
                    ConditionalRules[Condition].PrivateIncludePaths.Add(NormalizePath(PrivateIncludePath));
                }
            }
        }
        
        private void UpdateLibraries()
        {
            List<string> ConditionalLibraryPaths = new();
            foreach (string LibraryPath in ModuleInterface.PrivateLibraryPaths)
            {
                if (LibraryPath.Contains("$(Configuration)") || LibraryPath.Contains("$(Platform)"))
                {
                    ConditionalLibraryPaths.Add(LibraryPath);
                    continue;
                }
                PrivateLibraryPaths.Add(NormalizePath(LibraryPath));
            }

            foreach (ModuleCondition Condition in Project.Conditions)
            {
                // LibraryPaths
                {
                    foreach (string ConditionalLibraryPath in ConditionalLibraryPaths)
                    {
                        string LibraryPath = ConditionalLibraryPath.Replace("$(Configuration)", Condition.Configuration.ToString());
                        LibraryPath = LibraryPath.Replace("$(Platform)", Condition.Platform.ToString());
                        ConditionalRules[Condition].PrivateLibraryPaths.Add(NormalizePath(LibraryPath));
                    }

                    foreach (string PrivateLibraryPath in ModuleInterface.ConditionalRules[ModuleCondition.Default].PrivateLibraryPaths)
                    {
                        ConditionalRules[Condition].PrivateLibraryPaths.Add(NormalizePath(PrivateLibraryPath));
                    }

                    foreach (string PrivateLibraryPath in ModuleInterface.ConditionalRules[Condition].PrivateLibraryPaths)
                    {
                        ConditionalRules[Condition].PrivateLibraryPaths.Add(NormalizePath(PrivateLibraryPath));
                    }
                }
                
                // Libraries
                {
                    foreach (string PrivateLibrary in ModuleInterface.ConditionalRules[ModuleCondition.Default].PrivateLibraries)
                    {
                        ConditionalRules[Condition].PrivateLibraries.Add(NormalizePath(Condition, PrivateLibrary));
                    }

                    foreach (string PrivateLibrary in ModuleInterface.ConditionalRules[Condition].PrivateLibraries)
                    {
                        ConditionalRules[Condition].PrivateLibraries.Add(NormalizePath(Condition, PrivateLibrary));
                    }
                }
            }
        }
    }

    [DebuggerDisplay("{VirtualFolder}, {FullPath}")]
    public class CSharpModule : IProjectModule
    {
        public CSharpModule(Project Project, string VirtualFolder, string FullPath) : base(Project, VirtualFolder, ModuleCategory.Net, FullPath)
        {
        }
    }

    public class Project
    {
        public string FullPath = string.Empty;

        public List<string> Configurations = new() { "Debug", "Release" };
        public List<string> Platforms = new() { "Win32", "x64" };

        public List<ModuleCondition> Conditions = new()
        {
            ModuleCondition.Debug_x86,
            ModuleCondition.Debug_x64,
            ModuleCondition.Release_x86,
            ModuleCondition.Release_x64,
        };

        public List<string> WorkingRoots = new();

        public string ProjectRoot => Path.GetDirectoryName(FullPath)!;
        public string EngineRoot = string.Empty;

        public List<CPPModule> CPPModules = new();
        public List<CSharpModule> CSharpModules = new();

        public IEnumerable<IProjectModule> Modules
        {
            get
            {
                foreach (var CPPModule in CPPModules)
                    yield return CPPModule;
                foreach (var CSharpModule in CSharpModules)
                    yield return CSharpModule;
            }
        }

        public ProjectVirtualFolder VirtualRoot =  new(null, "__Root");

        public string Name => Path.GetFileNameWithoutExtension(FullPath)!;

        public string ActiveConfiguration => Configurations[0];
        public string ActivePlatform => Platforms[1];

        private readonly Stack<CPPModule> DiagnosisReferenceStack = new();

        [DebuggerDisplay("{VirtualFolder} {FilePath}")]
        public struct ModuleFile
        {
            public string FilePath;
            public string VirtualFolder;
        }

        public class XinFile
        {
            public string Version { get; set; } = string.Empty;
            public Dictionary<string, string> Imports { get; set; } = new();
        }

        public static Project? GenerateFromXinFile(string ProjectFilePath)
        {
            Project Project = new Project
            {
                FullPath = ProjectFilePath
            };

            using FileStream Reader = File.Open(ProjectFilePath, FileMode.Open);
            JsonSerializerOptions SerializerOptions = new JsonSerializerOptions();
            SerializerOptions.AllowTrailingCommas = true;
            XinFile? XinFile = JsonSerializer.Deserialize<XinFile>(Reader, SerializerOptions);
            if (XinFile == null)
                return null;

            List<ModuleFile> CPPModuleFiles = new List<ModuleFile>();
            List<ModuleFile> CSharpModuleFiles = new List<ModuleFile>();

            void TravalModules(string SourcesPath, string NextVirtualFolder, string VirtualFolder = "")
            {
                if (!Directory.Exists(SourcesPath))
                    return;

                List<string> CSharpModuleProjectFiles = Directory.EnumerateFiles(SourcesPath, "*.csproj").ToList();
                foreach (string CSharpModuleProjectFile in CSharpModuleProjectFiles)
                {
                    CSharpModuleFiles.Add(new ModuleFile() {FilePath = CSharpModuleProjectFile, VirtualFolder = VirtualFolder });
                    return;
                }

                string ModuleSourceFile = Path.Combine(SourcesPath, "Module.cs");
                if (File.Exists(ModuleSourceFile))
                {
                    CPPModuleFiles.Add(new ModuleFile() { FilePath = ModuleSourceFile, VirtualFolder = VirtualFolder });
                    return;
                }

                List<string> ModuleFileEnumerable = Directory.EnumerateFiles(SourcesPath, "*.Module.cs").ToList();
                foreach(string ModuleFile in ModuleFileEnumerable)
                    CPPModuleFiles.Add(new ModuleFile() { FilePath = ModuleFile, VirtualFolder = VirtualFolder} );

                foreach (var SubDiretory in Directory.EnumerateDirectories(SourcesPath))
                    TravalModules(SubDiretory, NextVirtualFolder + Path.DirectorySeparatorChar + Path.GetFileName(SubDiretory), NextVirtualFolder);
            }

            string CombineVirtualFolder(string? ParentName, string Name)
            {
                return string.IsNullOrEmpty(ParentName) ? Name : Path.Combine(ParentName, Name);
            }

            void TravalProject(string ProjectPath, string VirtualFolder)
            {
                if (!Path.IsPathRooted(ProjectPath))
                    ProjectPath = Path.GetFullPath(ProjectPath, Project.ProjectRoot);
                Project.WorkingRoots.Add(ProjectPath);

                TravalModules(Path.Combine(ProjectPath, "Sources", "Development"), CombineVirtualFolder(VirtualFolder, "Development"));
                TravalModules(Path.Combine(ProjectPath, "Sources", "Externals"), CombineVirtualFolder(VirtualFolder, "Externals"));
                TravalModules(Path.Combine(ProjectPath, "Sources", "Depends"), CombineVirtualFolder(VirtualFolder, "Depends"));
                TravalModules(Path.Combine(ProjectPath, "Sources", "Runtimes"), CombineVirtualFolder(VirtualFolder, "Runtimes"));
                TravalModules(Path.Combine(ProjectPath, "Sources", "Plugins"), CombineVirtualFolder(VirtualFolder, "Plugins"));
                TravalModules(Path.Combine(ProjectPath, "Sources", "Programs"), CombineVirtualFolder(VirtualFolder, "Programs"));
                TravalModules(Path.Combine(ProjectPath, "Sources", "Tools"), CombineVirtualFolder(VirtualFolder, "Tools"));
            }

            if (XinFile.Imports.Any())
            {
                foreach (var Import in XinFile.Imports)
                    TravalProject(Path.GetDirectoryName(Import.Value)!, Import.Key);
                TravalProject(Path.GetDirectoryName(ProjectFilePath)!, "Game");
            }
            else
                TravalProject(Path.GetDirectoryName(ProjectFilePath)!, string.Empty);

            foreach (string ProjectRoot in Project.WorkingRoots)
            {
                if (Path.GetFileName(ProjectRoot).Equals("Xin"))
                {
                    Project.EngineRoot = ProjectRoot;
                    break;
                }
            }

            if (string.IsNullOrEmpty(Project.EngineRoot))
                return null;
            
            foreach (ModuleFile CPPModuleFile in CPPModuleFiles)
            {
                IModule? CompiledModule = ModuleCompiler.CompileModuleFromModuleFile(Project.ProjectRoot, CPPModuleFile.FilePath);
                if (CompiledModule == null)
                    continue;

                var Module = new CPPModule(Project, CPPModuleFile.VirtualFolder, CompiledModule);
                Project.CPPModules.Add(Module);
            }

            foreach (ModuleFile CSharpModuleFile in CSharpModuleFiles)
            {
                Project.CSharpModules.Add(new CSharpModule(Project, CSharpModuleFile.VirtualFolder, CSharpModuleFile.FilePath));
            }

            Project.UpdateDependencyModules();
            Project.UpdateVirutalFolders();
            return Project;
        }

        public CPPModule? FindModule(string ModuleName)
        {
            if (string.IsNullOrEmpty(ModuleName))
                return null;

            foreach (CPPModule Module in CPPModules)
            {
                if (Module.Name == ModuleName || Module.Name == "Xin." + ModuleName)
                    return Module;
            }

            //foreach (CPPModule Module in CPPModules)
            //{
            //    if (Module.Name.EndsWith("." + ModuleName))
            //        return Module;
            //}
            return null;
        }

        private void UpdateDependencyModules()
        {
            foreach (var Module in CPPModules)
            {
                foreach (string ModuleName in Module.ModuleInterface.PrivateDependencyModules)
                {
                    CPPModule? DependencyModule = FindModule(ModuleName);
                    if (DependencyModule != null)
                        Module.PrivateDependencyModules.Add(DependencyModule);
                }

                foreach (string ModuleName in Module.ModuleInterface.PublicDependencyModules)
                {
                    CPPModule? DependencyModule = FindModule(ModuleName);
                    if (DependencyModule != null)
                        Module.PublicDependencyModules.Add(DependencyModule);
                }
            }

            foreach (var Module in CPPModules)
                UpdateDependencyModules(Module);
        }

        private void UpdateDependencyModules(CPPModule Module)
        {
            DiagnosisReferenceStack.Clear();

            foreach (var PrivateDependencyModule in Module.PrivateDependencyModules)
                Module.DependencyModules.Add(PrivateDependencyModule);

            foreach (var PublicDependencyModule in Module.PublicDependencyModules)
            {
                DiagnosisReferenceStack.Push(PublicDependencyModule);
                UpdateIndirectDependencyModules(Module, PublicDependencyModule);
                DiagnosisReferenceStack.Pop();
            }
        }

        private void UpdateIndirectDependencyModules(CPPModule Module, CPPModule PublicDependencyModule)
        {
            if (Module.DependencyModules.Contains(PublicDependencyModule))
                return;

            if (PublicDependencyModule == Module)
            {
                Console.WriteLine($"Module \"{Module.Name}\" reference \"{PublicDependencyModule.Name}\" recursively.");
                foreach(var ReferencedProject in DiagnosisReferenceStack)
                    Console.WriteLine($"\t{ReferencedProject.Name}");
                return;
            }

            Module.DependencyModules.Add(PublicDependencyModule);

            foreach (var IndirectDependencyModule in PublicDependencyModule.PublicDependencyModules)
            {
                DiagnosisReferenceStack.Push(PublicDependencyModule);
                UpdateIndirectDependencyModules(Module, IndirectDependencyModule);
                DiagnosisReferenceStack.Pop();
            }
        }

        private void UpdateVirutalFolders()
        {
            foreach (IProjectModule Module in Modules)
            {
                ProjectVirtualFolder CurrentFolder = VirtualRoot;
                string[] VirtualNames = Module.VirtualFolder.Split(Path.DirectorySeparatorChar);
                for (int NameIndex = 0; NameIndex < VirtualNames.Length; ++NameIndex)
                {
                    string VirtualName = VirtualNames[NameIndex];
                    ProjectVirtualFolder? FoundFolder = CurrentFolder.Children.Find(X => X.Name.Equals(VirtualName));
                    if (FoundFolder == null)
                    {
                        FoundFolder = new ProjectVirtualFolder(CurrentFolder, VirtualName);
                        CurrentFolder.Children.Add(FoundFolder);
                    }
                    CurrentFolder = FoundFolder;
                }
                CurrentFolder.Modules.Add(Module);
            }
        }
    }
}
