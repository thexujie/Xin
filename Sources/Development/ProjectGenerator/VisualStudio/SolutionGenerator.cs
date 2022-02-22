using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Reflection;
using System.Security.Cryptography;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Xml;
using System.Xml.Linq;
using System.Xml.Schema;
using System.Xml.Serialization;
using Microsoft.CodeAnalysis;
using ProjectModel;

namespace ProjectGenerator.VisualStudio;

public sealed class Utf8StringWriter : StringWriter
{
    public override Encoding Encoding => Encoding.UTF8;
}

internal class SolutionGenerator
{
    readonly XmlWriterSettings XmlWriterSettings = new XmlWriterSettings 
    {
        Encoding = Encoding.UTF8,
        Indent = true,
        IndentChars = "  ",
        OmitXmlDeclaration = false,
    };

    public string? WindowsTargetPlatformVersion = /*"10.0"*/null;
    public string VCProjectVersion = "17.0";

    private static string GetConfigurationTypeString(ModuleType Type)
    {
        switch (Type)
        {
            case ModuleType.DynamicLibrary:
                return "DynamicLibrary";
            case ModuleType.StaticLibrary:
                return "StaticLibrary";
            case ModuleType.Application:
                return "Application";
            case ModuleType.Makefile:
                return "Makefile";
            default:
                return "None";
        }
    }

    readonly MD5 MD5Computer = MD5.Create();

    readonly string CPPDefaultPropsFileRelativePath = "$(SolutionDir)Documents\\Build\\MSBuild\\Cpp.Default.props";
    readonly string CPPDerivedPropsFileRelativePath = "$(SolutionDir)Documents\\Build\\MSBuild\\Cpp.Derived.props";

    private void InitializePropFiles(Project Project)
    {
        string EngineMSBuildRoot = Path.Combine(Project.EngineRoot, "Sources", "Development", "Build", "MSBuild");
        string ProjectMSBuildRoot = Path.Combine(Project.ProjectRoot, "Documents", "Build", "MSBuild");
        if (!Directory.Exists(ProjectMSBuildRoot))
            Directory.CreateDirectory(ProjectMSBuildRoot);

        string EngineRoot = Project.EngineRoot + Path.DirectorySeparatorChar;
        string ProjectRoot = Project.ProjectRoot + Path.DirectorySeparatorChar;

        string CPPDefaultPropsFilePath = Path.Combine(Project.ProjectRoot, "Documents", "Build", "MSBuild", "Cpp.Default.props");
        string CPPDerivedPropsFilePath = Path.Combine(Project.ProjectRoot, "Documents", "Build", "MSBuild", "Cpp.Derived.props");

        // Cpp.Default.props
        {
            using StringWriter SWriter = new Utf8StringWriter();
            using XmlWriter Writer = XmlWriter.Create(SWriter, XmlWriterSettings);

            // Begin
            Writer.WriteStartDocument();
            Writer.WriteStartElement("Project", "http://schemas.microsoft.com/developer/msbuild/2003");
            Writer.WriteAttributeString("DefaultTargets", "Build");
            Writer.WriteAttributeString("ToolsVersion", "15.0");

            {
                Writer.WriteStartElement("PropertyGroup");
                Writer.WriteElementString("EngineRoot", EngineRoot);
                Writer.WriteElementString("ProjectRoot", ProjectRoot);
                Writer.WriteEndElement();
            }

            {
                Writer.WriteStartElement("Import");
                Writer.WriteAttributeString("Project", "$(EngineRoot)Sources\\Development\\MSBuild\\Cpp.Default.props");
                Writer.WriteEndElement();
            }

            // End
            Writer.WriteEndElement();
            Writer.WriteEndDocument();
            Writer.Close();
            SWriter.Close();

            string CPPDefaultPropsFileContent = SWriter.ToString();
            if (!File.Exists(CPPDefaultPropsFilePath) || !File.ReadAllText(CPPDefaultPropsFilePath).Equals(CPPDefaultPropsFileContent))
                File.WriteAllText(CPPDefaultPropsFilePath, CPPDefaultPropsFileContent, Encoding.UTF8);
        }

        // Cpp.Derived.props
        {
            using StringWriter SWriter = new Utf8StringWriter();
            using XmlWriter Writer = XmlWriter.Create(SWriter, XmlWriterSettings);

            // Begin
            Writer.WriteStartDocument();
            Writer.WriteStartElement("Project", "http://schemas.microsoft.com/developer/msbuild/2003");
            Writer.WriteAttributeString("DefaultTargets", "Build");
            Writer.WriteAttributeString("ToolsVersion", "15.0");
            
            {
                Writer.WriteStartElement("Import");
                Writer.WriteAttributeString("Project", "$(EngineRoot)Sources\\Development\\MSBuild\\Cpp.Derived.props");
                Writer.WriteEndElement();
            }

            // End
            Writer.WriteEndElement();
            Writer.WriteEndDocument();
            Writer.Close();
            SWriter.Close();

            string CPPDerivedPropsFileContent = SWriter.ToString();
            if (!File.Exists(CPPDerivedPropsFilePath) || !File.ReadAllText(CPPDerivedPropsFilePath).Equals(CPPDerivedPropsFileContent))
                File.WriteAllText(CPPDerivedPropsFilePath, CPPDerivedPropsFileContent, Encoding.UTF8);
        }
    }


    readonly List<ProjectModuleFile> HeaderFiles = new List<ProjectModuleFile>();
    readonly List<ProjectModuleFile> SourceFiles = new List<ProjectModuleFile>();
    readonly List<ProjectModuleFile> MiscFiles = new List<ProjectModuleFile>();

    public bool GenerateVcxproj(CPPModule Module)
    {
        HeaderFiles.Clear();
        SourceFiles.Clear();
        MiscFiles.Clear();

        foreach (ProjectModuleFile File in Module.Files)
        {
            switch (File.FileType)
            {
                case ProjectModuleFile.FileType_H:
                case ProjectModuleFile.FileType_HPP:
                    HeaderFiles.Add(File);
                    break;
                case ProjectModuleFile.FileType_C:
                case ProjectModuleFile.FileType_CC:
                case ProjectModuleFile.FileType_CPP:
                    SourceFiles.Add(File);
                    break;
                default:
                    MiscFiles.Add(File);
                    break;
            }
        }

        WriteVcxproj(Module);
        WriteVcxprojFilters(Module);
        return true;
    }

    private bool WriteVcxproj(CPPModule Module)
    {
        //string VcxprojFilePath = Path.Combine(Path.GetDirectoryName(Module.FullPath)!, Module.Name + ".test.xml");
        string VcxprojFilePath = Path.Combine(Module.ModuleRoot, Module.Name + ".vcxproj");
        string MD5StringExists = string.Empty;
        if (File.Exists(VcxprojFilePath))
            MD5StringExists = Convert.ToHexString(MD5Computer.ComputeHash(Encoding.ASCII.GetBytes(File.ReadAllText(VcxprojFilePath))));


        using StringWriter SWriter = new Utf8StringWriter();
        using XmlWriter Writer = XmlWriter.Create(SWriter, XmlWriterSettings);
        // Begin
        Writer.WriteStartDocument();
        Writer.WriteStartElement("Project", "http://schemas.microsoft.com/developer/msbuild/2003");
        Writer.WriteAttributeString("DefaultTargets", "Build");
        Writer.WriteAttributeString("ToolsVersion", "15.0");

        // ProjectConfigurations
        {
            Writer.WriteStartElement("ItemGroup");
            Writer.WriteAttributeString("Label", "ProjectConfigurations");
            foreach (string Configuration in Module.Project.Configurations)
            {
                foreach (string Platform in Module.Project.Platforms)
                {
                    Writer.WriteStartElement("ProjectConfiguration");
                    Writer.WriteAttributeString("Include", Configuration + "|" + Platform);
                    Writer.WriteElementString("Configuration", Configuration);
                    Writer.WriteElementString("Platform", Platform);
                    Writer.WriteEndElement();
                }
            }
            Writer.WriteEndElement();
        }

        // Globals
        {
            Writer.WriteStartElement("PropertyGroup");
            Writer.WriteAttributeString("Label", "Globals");
            if (Module.Type == ModuleType.Makefile)
                Writer.WriteElementString("VCProjectVersion", VCProjectVersion);
            Writer.WriteElementString("ProjectGuid", ProjectGuids[Module.FullPath].ToString("B").ToUpper());
            if (WindowsTargetPlatformVersion != null)
                Writer.WriteElementString("WindowsTargetPlatformVersion", WindowsTargetPlatformVersion);
            if (Module.SubType != ModuleSubType.None)
                Writer.WriteElementString("ProjectSubType", Module.SubType.ToString());
            Writer.WriteEndElement();
        }

        // Cpp.Default.props
        {
            Writer.WriteStartElement("Import");
            Writer.WriteAttributeString("Project", CPPDefaultPropsFileRelativePath);
            Writer.WriteEndElement();

            Writer.WriteStartElement("Import");
            Writer.WriteAttributeString("Project", "$(VCTargetsPath)\\Microsoft.Cpp.Default.props");
            Writer.WriteEndElement();
        }

        // Configuration
        foreach (string Configuration in Module.Project.Configurations)
        {
            foreach (string Platform in Module.Project.Platforms)
            {
                Writer.WriteStartElement("PropertyGroup");
                Writer.WriteAttributeString("Condition", $"'$(Configuration)|$(Platform)'=='{Configuration}|{Platform}'");
                Writer.WriteAttributeString("Label", "Configuration");
                Writer.WriteElementString("ConfigurationType", GetConfigurationTypeString(Module.Type));
                Writer.WriteEndElement();
            }
        }

        // Cpp.Derived.props
        {
            Writer.WriteStartElement("Import");
            Writer.WriteAttributeString("Project", "$(VCTargetsPath)\\Microsoft.Cpp.props");
            Writer.WriteEndElement();

            Writer.WriteStartElement("Import");
            Writer.WriteAttributeString("Project", CPPDerivedPropsFileRelativePath);
            Writer.WriteEndElement();
        }

        // Properties
        foreach (string Configuration in Module.Project.Configurations)
        {
            foreach (string Platform in Module.Project.Platforms)
            {
                Writer.WriteStartElement("PropertyGroup");
                Writer.WriteAttributeString("Condition", $"'$(Configuration)|$(Platform)'=='{Configuration}|{Platform}'");
                Writer.WriteEndElement();
            }
        }

        if (Module.Type == ModuleType.StaticLibrary || Module.Type == ModuleType.DynamicLibrary || Module.Type == ModuleType.Application)
        {
            // ItemDefinitionGroup(s)
            foreach (ModuleCondition Condition in Module.Project.Conditions)
            {
                Writer.WriteStartElement("ItemDefinitionGroup");
                Writer.WriteAttributeString("Condition", $"'$(Configuration)|$(Platform)'=='{Condition.Configuration}|{Condition.Platform}'");
                {
                    // ClCompile
                    {
                        Writer.WriteStartElement("ClCompile");
                        // AdditionalIncludeDirectories
                        List<string> AdditionalIncludeDirectories = Module.ConditionalPrivateIncludePaths(Condition).ToList();
                        if (AdditionalIncludeDirectories.Any())
                        {
                            StringBuilder AdditionalIncludeDirectoriesSB = new StringBuilder();
                            foreach (string PrivateIncludePath in AdditionalIncludeDirectories)
                            {
                                if (AdditionalIncludeDirectoriesSB.Length > 0)
                                    AdditionalIncludeDirectoriesSB.Append(";");
                                AdditionalIncludeDirectoriesSB.Append(PrivateIncludePath);
                            }
                            AdditionalIncludeDirectoriesSB.Append(";%(AdditionalIncludeDirectories)");
                            Writer.WriteElementString("AdditionalIncludeDirectories", AdditionalIncludeDirectoriesSB.ToString());
                        }

                        if (Module.PrivateDefines.Any() || Module.PublicDefines.Any())
                        {
                            StringBuilder PreprocessorDefinitionsSB = new StringBuilder();
                            foreach (string PrivateDefine in Module.PrivateDefines)
                            {
                                if (PreprocessorDefinitionsSB.Length > 0)
                                    PreprocessorDefinitionsSB.Append(";");
                                PreprocessorDefinitionsSB.Append(PrivateDefine);
                            }
                            foreach (string PublicDefine in Module.PublicDefines)
                            {
                                if (PreprocessorDefinitionsSB.Length > 0)
                                    PreprocessorDefinitionsSB.Append(";");
                                PreprocessorDefinitionsSB.Append(PublicDefine);
                            }
                            PreprocessorDefinitionsSB.Append(";%(PreprocessorDefinitions)");
                            Writer.WriteElementString("PreprocessorDefinitions", PreprocessorDefinitionsSB.ToString());
                        }
                        Writer.WriteEndElement();
                    }
                    // Link
                    {
                        Writer.WriteStartElement("Link");
                        List<string> AdditionalLibraryDirectories = Module.ConditionalPrivateLibraryPaths(Condition).ToList();
                        if (AdditionalLibraryDirectories.Any())
                        {
                            // AdditionalLibraryDirectories
                            if (AdditionalLibraryDirectories.Any())
                            {
                                StringBuilder AdditionalLibraryDirectoriesSB = new StringBuilder();
                                foreach (string PrivateLibraryPath in AdditionalLibraryDirectories)
                                {
                                    if (AdditionalLibraryDirectoriesSB.Length > 0)
                                        AdditionalLibraryDirectoriesSB.Append(";");
                                    AdditionalLibraryDirectoriesSB.Append(PrivateLibraryPath);
                                }
                                AdditionalLibraryDirectoriesSB.Append(";%(AdditionalLibraryDirectories)");
                                Writer.WriteElementString("AdditionalLibraryDirectories", AdditionalLibraryDirectoriesSB.ToString());
                            }
                        }
                        List<string> PrivateLibrarys = Module.ConditionalPrivateLibrarys(Condition).ToList();
                        if (PrivateLibrarys.Any())
                        {
                            // PrivateLibrarys
                            if (PrivateLibrarys.Any())
                            {
                                StringBuilder PrivateLibrarysSB = new StringBuilder();
                                foreach (string PrivateLibrary in PrivateLibrarys)
                                {
                                    if (PrivateLibrarysSB.Length > 0)
                                        PrivateLibrarysSB.Append(";");
                                    PrivateLibrarysSB.Append(PrivateLibrary);
                                }
                                PrivateLibrarysSB.Append(";%(AdditionalDependencies)");
                                Writer.WriteElementString("AdditionalDependencies", PrivateLibrarysSB.ToString());
                            }
                        }
                        Writer.WriteElementString("SubSystem", "Console");
                        Writer.WriteEndElement(); // Link
                    }
                }
                Writer.WriteEndElement();
            }
        }

        // Files
        {
            // ClInclude
            if (HeaderFiles.Any())
            {
                Writer.WriteStartElement("ItemGroup");
                foreach (ProjectModuleFile File in HeaderFiles)
                {
                    Writer.WriteStartElement("ClInclude");
                    Writer.WriteAttributeString("Include", File.FilePath);
                    Writer.WriteEndElement();
                }
                Writer.WriteEndElement();
            }

            // ClCompile
            if (SourceFiles.Any())
            {
                Writer.WriteStartElement("ItemGroup");
                foreach (ProjectModuleFile File in SourceFiles)
                {
                    Writer.WriteStartElement("ClCompile");
                    Writer.WriteAttributeString("Include", File.FilePath);
                    if (File.FilePath == Module.PCHFile)
                        Writer.WriteElementString("PrecompiledHeader", "Create");
                    Writer.WriteEndElement();
                }
                Writer.WriteEndElement();
            }

            // None
            if (MiscFiles.Any())
            {
                Writer.WriteStartElement("ItemGroup");
                foreach (ProjectModuleFile File in MiscFiles)
                {
                    Writer.WriteStartElement("None");
                    Writer.WriteAttributeString("Include", File.FilePath);
                    Writer.WriteEndElement();
                }
                Writer.WriteEndElement();
            }
        }

        // Microsoft.Cpp.targets
        {
            Writer.WriteStartElement("Import");
            Writer.WriteAttributeString("Project", "$(VCTargetsPath)\\Microsoft.Cpp.targets");
            Writer.WriteEndElement();
        }

        // ExtensionTargets
        {
            Writer.WriteStartElement("ImportGroup");
            Writer.WriteAttributeString("Label", "ExtensionTargets");
            Writer.WriteEndElement();
        }

        // Dependency Modules
        if (Module.DependencyModules.Any())
        {
            List<CPPModule> SortedDependencyModules = Module.DependencyModules.ToList();
            SortedDependencyModules.Sort((A, B)=>A.Name.CompareTo(B.Name));

            Writer.WriteStartElement("ItemGroup");
            foreach (CPPModule DependencyModule in SortedDependencyModules)
            {
                string DependencyModuleRelativePath = Path.GetRelativePath(Module.ModuleRoot, DependencyModule.ModuleRoot);
                Writer.WriteStartElement("ProjectReference");
                Writer.WriteAttributeString("Include", Path.Combine(DependencyModuleRelativePath, DependencyModule.Name + ".vcxproj"));
                Writer.WriteElementString("Project", ProjectGuids[DependencyModule.FullPath].ToString("B").ToUpper());
                Writer.WriteEndElement();
            }
            Writer.WriteEndElement();
        }

        // End
        Writer.WriteEndElement();
        Writer.WriteEndDocument();

        Writer.Close();
        SWriter.Close();

        string ProjectFileContent = SWriter.ToString();
        string ProjectMd5String = Convert.ToHexString(MD5Computer.ComputeHash(Encoding.ASCII.GetBytes(ProjectFileContent)));
        if (ProjectMd5String.Equals(MD5StringExists))
            return true;

        File.WriteAllText(VcxprojFilePath, ProjectFileContent, Encoding.UTF8);
        return true;
    }

    private bool WriteVcxprojFilters(CPPModule Module)
    {
        Dictionary<string, Guid> FilterGuids = new();
        foreach (ProjectModuleFile File in Module.Files)
        {
            if (!string.IsNullOrEmpty(File.Filter))
            {
                string Filter = File.Filter.Replace('/', '\\');
                while (true)
                {
                    FilterGuids[Filter] = Guid.Empty;
                    int Index = Filter.LastIndexOf('\\');
                    if (Index == -1 || Index == 0)
                        break;

                    Filter = Filter.Substring(0, Index);
                }
            }
        }

        string MD5StringExists = string.Empty;
        string FilterFilePath = Path.Combine(Module.ModuleRoot, Module.Name + ".vcxproj.filters");
        if (File.Exists(FilterFilePath))
        {
            if (!FilterGuids.Any())
            {
                File.Delete(FilterFilePath);
                return true;
            }

            string ExistsFilterFileContent = File.ReadAllText(FilterFilePath);
            MD5StringExists = Convert.ToHexString(MD5Computer.ComputeHash(Encoding.ASCII.GetBytes(ExistsFilterFileContent)));

            XmlDocument Document = new XmlDocument();
            Document.LoadXml(ExistsFilterFileContent);

            XmlNamespaceManager Namespaces = new XmlNamespaceManager(Document.NameTable);
            Namespaces.AddNamespace("ns", "http://schemas.microsoft.com/developer/msbuild/2003");

            XmlNodeList? FilterNodes = Document.DocumentElement?.SelectNodes("//ns:Project/ns:ItemGroup/ns:Filter", Namespaces);
            if (FilterNodes != null)
            {
                foreach (XmlElement? FilterNode in FilterNodes)
                {
                    if (FilterNode == null)
                        continue;

                    string FilterFolder = FilterNode.GetAttribute("Include");
                    XmlNode? UniqueIdentifierNode = FilterNode.SelectSingleNode("ns:UniqueIdentifier", Namespaces);
                    if (UniqueIdentifierNode == null)
                        continue;

                    if (FilterGuids.ContainsKey(FilterFolder))
                    {
                        Guid FilterGuid = Guid.Parse(UniqueIdentifierNode.InnerText);
                        FilterGuids[FilterFolder] = FilterGuid;
                    }
                }
            }
        }

        if (!FilterGuids.Any())
            return true;

        foreach (KeyValuePair<string, Guid> FilterGuid in FilterGuids)
        {
            if (FilterGuid.Value == Guid.Empty)
                FilterGuids[FilterGuid.Key] = Guid.NewGuid();
        }

        // ----------------------------------------------------------------
        // ----------------------------------------------------------------
        // ----------------------------------------------------------------
        XmlWriterSettings WriterSettings = new XmlWriterSettings {
            Encoding = Encoding.UTF8,
            Indent = true,
            IndentChars = "  ",
            OmitXmlDeclaration = false,
        };
        using StringWriter SWriter = new Utf8StringWriter();
        using XmlWriter Writer = XmlWriter.Create(SWriter, WriterSettings);

        // Begin
        Writer.WriteStartDocument();
        Writer.WriteStartElement("Project", "http://schemas.microsoft.com/developer/msbuild/2003");
        Writer.WriteAttributeString("ToolsVersion", "4.0");

        // ClInclude
        if (HeaderFiles.Any())
        {
            Writer.WriteStartElement("ItemGroup");
            foreach (ProjectModuleFile File in HeaderFiles)
            {
                Writer.WriteStartElement("ClInclude");
                Writer.WriteAttributeString("Include", File.FilePath);
                if (!string.IsNullOrEmpty(File.Filter))
                    Writer.WriteElementString("Filter", File.Filter);
                Writer.WriteEndElement();
            }
            Writer.WriteEndElement();
        }

        // ClCompile
        if (SourceFiles.Any())
        {
            Writer.WriteStartElement("ItemGroup");
            foreach (ProjectModuleFile File in SourceFiles)
            {
                Writer.WriteStartElement("ClCompile");
                Writer.WriteAttributeString("Include", File.FilePath);
                if (!string.IsNullOrEmpty(File.Filter))
                    Writer.WriteElementString("Filter", File.Filter);
                Writer.WriteEndElement();
            }
            Writer.WriteEndElement();
        }

        // None
        if (MiscFiles.Any())
        {
            Writer.WriteStartElement("ItemGroup");
            foreach (ProjectModuleFile File in MiscFiles)
            {
                Writer.WriteStartElement("None");
                Writer.WriteAttributeString("Include", File.FilePath);
                if (!string.IsNullOrEmpty(File.Filter))
                    Writer.WriteElementString("Filter", File.Filter);
                Writer.WriteEndElement();
            }
            Writer.WriteEndElement();
        }

        // UniqueIdentifiers
        {
            List<string> Filters = new List<string>();
            foreach (KeyValuePair<string, Guid> FilterGuid in FilterGuids)
                Filters.Add(FilterGuid.Key);
            Filters.Sort();

            Writer.WriteStartElement("ItemGroup");
            foreach (string Filter in Filters)
            {
                Writer.WriteStartElement("Filter");
                Writer.WriteAttributeString("Include", Filter);
                Writer.WriteElementString("UniqueIdentifier", FilterGuids[Filter].ToString("B"));
                Writer.WriteEndElement();
            }
            Writer.WriteEndElement();
        }

        // End
        Writer.WriteEndElement();
        Writer.WriteEndDocument();

        Writer.Close();
        SWriter.Close();

        string FilterFileContent = SWriter.ToString();
        if (!string.IsNullOrEmpty(MD5StringExists))
        {
            string MD5StringThis = Convert.ToHexString(MD5Computer.ComputeHash(Encoding.ASCII.GetBytes(FilterFileContent)));
            if (MD5StringThis.Equals(MD5StringExists))
                return true;
        }
        File.WriteAllText(FilterFilePath, FilterFileContent, Encoding.UTF8);
        return false;
    }


    private readonly string SolutionFileVersion = "12.00";
    private readonly string SolutionFileVisualStudioMajorVersion = "17";
    private readonly string SolutionFileVisualStudioVersion = "17.0.31912.275";
    private readonly string MinimumVisualStudioVersion = "10.0.40219.1";
    private readonly Dictionary<string, Guid> KnownVirtualFolderGuids = new Dictionary<string, Guid>();

    private readonly HashSet<Guid> KnownGuids = new HashSet<Guid>();
    private readonly Dictionary<string, Guid> ProjectGuids = new Dictionary<string, Guid>();

    private void UpdateKnonwSolutionProperties(Project Project, string SolutionFileContent)
    {
        Dictionary<Guid, Guid> VirtualFolderParentGuids = new Dictionary<Guid, Guid>();
        {
            Regex RegexNestedFolders = new Regex(@"\s?(?<FolderGuid>\w{8}-\w{4}-\w{4}-\w{4}-\w{12})\}\s?"
                + @"=\s?\{(?<ParentFolderGuid>\w{8}-\w{4}-\w{4}-\w{4}-\w{12})\}");
            MatchCollection Matches = RegexNestedFolders.Matches(SolutionFileContent);
            foreach (Match Match in Matches)
            {
                if (!Match.Success)
                    continue;

                if (Match.Groups["VirtualFolder0"].Value.Equals(Match.Groups["VirtualFolder1"].Value))
                {
                    Guid FolderGuid = Guid.Parse(Match.Groups["FolderGuid"].Value);
                    Guid ParentFolderGuid = Guid.Parse(Match.Groups["ParentFolderGuid"].Value);
                    VirtualFolderParentGuids[FolderGuid] = ParentFolderGuid;
                }
            }
        }
        Dictionary<Guid, string> VirtualFolderGuidNames = new Dictionary<Guid, string>();
        {
            Regex RegexFolders = new Regex(@"Project\(\""{2150E333-8FDC-42A3-9474-1A3956D46DE8}\""\)\s*"
                + @"=\s*\""(?<VirtualFolder0>\w+)""\s*,\s*\""(?<VirtualFolder1>\w+)"""
                + @",\s*\""\{(?<FolderGuid>\w{8}-\w{4}-\w{4}-\w{4}-\w{12})\}\""");
            MatchCollection Matches = RegexFolders.Matches(SolutionFileContent);
            foreach (Match Match in Matches)
            {
                if (!Match.Success)
                    continue;

                if (Match.Groups["VirtualFolder0"].Value.Equals(Match.Groups["VirtualFolder1"].Value))
                {
                    string VirtualFolder = Match.Groups["VirtualFolder0"].Value;
                    Guid FolderGuid = Guid.Parse(Match.Groups["FolderGuid"].Value);
                    VirtualFolderGuidNames[FolderGuid] = VirtualFolder;
                }
            }
        }

        string GetVirtualFolderFullPath(Guid FolderGuid)
        {
            if (VirtualFolderGuidNames.TryGetValue(FolderGuid, out string? VirtualFolderName))
            {
                if (VirtualFolderParentGuids.TryGetValue(FolderGuid, out Guid ParentFolderGuid))
                    return Path.Combine(GetVirtualFolderFullPath(ParentFolderGuid), VirtualFolderName);
                return VirtualFolderName;
            }
            return string.Empty;
        }

        foreach (KeyValuePair<Guid, string> VirtualFolderGuidName in VirtualFolderGuidNames)
        {
            string VirtualFolder = GetVirtualFolderFullPath(VirtualFolderGuidName.Key);
            if (!string.IsNullOrEmpty(VirtualFolder))
                KnownVirtualFolderGuids[VirtualFolder] = VirtualFolderGuidName.Key;
        }

        // Project Guids
        {
            Guid VirtualFolderCategaryGuid = Guid.Parse(Utils.GetModuleCategaryGuid(ModuleCategory.Folder));

            Regex RegexProjects = new Regex(@"Project\(\""{(?<CategaryGuid>\w{8}-\w{4}-\w{4}-\w{4}-\w{12})}\""\)\s*"
                +@"=\s*\""(?<ProjectName>.+?)\""\s*,\s*\""(?<ProjectFilePath>.+?)\"""
                +@",\s*\""\{(?<ProjectGuid>\w{8}-\w{4}-\w{4}-\w{4}-\w{12})\}\""");
            MatchCollection Matches = RegexProjects.Matches(SolutionFileContent);

            foreach (Match Match in Matches)
            {
                if (!Match.Success)
                    continue;

                Guid CategaryGuid = Guid.Parse(Match.Groups["CategaryGuid"].Value);
                if (CategaryGuid != VirtualFolderCategaryGuid)
                {
                    string ProjectFilePath = Match.Groups["ProjectFilePath"].Value;
                    if (!Path.IsPathRooted(ProjectFilePath))
                        ProjectFilePath = Path.GetFullPath(ProjectFilePath, Project.ProjectRoot);

                    Guid ProjectGuid = Guid.Parse(Match.Groups["ProjectGuid"].Value);
                    if (!KnownGuids.Contains(ProjectGuid))
                    {
                        ProjectGuids[ProjectFilePath] = ProjectGuid;
                        KnownGuids.Add(ProjectGuid);
                    }
                }
            }
        }
    }

    private void UpdateKnonwCPPModuleProperties(CPPModule CPPModule)
    {
        XmlDocument Document = new XmlDocument();
        Document.Load(CPPModule.VcxprojFilePath);

        XmlNamespaceManager Namespaces = new XmlNamespaceManager(Document.NameTable);
        Namespaces.AddNamespace("ns", "http://schemas.microsoft.com/developer/msbuild/2003");

        XmlNode? ProjectGuidNode = Document.DocumentElement?.SelectSingleNode("//ns:Project/ns:PropertyGroup[@Label=\"Globals\"]/ns:ProjectGuid", Namespaces);
        if (ProjectGuidNode != null)
        {
            Guid ProjectGuid = Guid.Parse(ProjectGuidNode.InnerText);
            if (!ProjectGuids.ContainsKey(CPPModule.VcxprojFilePath))
            {
                if (KnownGuids.Contains(ProjectGuid))
                {
                    ProjectGuid = Guid.NewGuid();
                    KnownGuids.Add(ProjectGuid);
                }
            }
            ProjectGuids[CPPModule.VcxprojFilePath] = ProjectGuid;
            //if (KnownProjectGuids.TryGetValue(CPPModule.VcxprojFilePath, out Guid KnownProjectGuid))
            //{
            //    if (ProjectGuid != KnownProjectGuid)
            //        KnownProjectGuids[CPPModule.VcxprojFilePath] = ProjectGuid;
            //}
        }
    }

    public bool GenerateSolution(Project Project)
    {
        // Initialize
        InitializePropFiles(Project);

        string SolutionFilePath = Path.Combine(Project.ProjectRoot, Project.Name + ".sln");
        string MD5StringExists = string.Empty;
        if (File.Exists(SolutionFilePath))
        {
            string KnownSolutionFileContent = File.ReadAllText(SolutionFilePath);
            MD5StringExists = Convert.ToHexString(MD5Computer.ComputeHash(Encoding.ASCII.GetBytes(KnownSolutionFileContent)));
            UpdateKnonwSolutionProperties(Project, KnownSolutionFileContent);
        }

        void InitializeVirtualFolderGuids(List<ProjectVirtualFolder> VirtualFolders)
        {
            foreach (ProjectVirtualFolder VirtualFolder in VirtualFolders)
            {
                if (!KnownVirtualFolderGuids.ContainsKey(VirtualFolder.FullPath))
                    KnownVirtualFolderGuids[VirtualFolder.FullPath] = Guid.NewGuid();
                InitializeVirtualFolderGuids(VirtualFolder.Children);
            }
        }
        InitializeVirtualFolderGuids(Project.VirtualRoot.Children);

        foreach (CPPModule CPPModule in Project.CPPModules)
        {
            if (File.Exists(CPPModule.VcxprojFilePath))
                UpdateKnonwCPPModuleProperties(CPPModule);

            if (!ProjectGuids.ContainsKey(CPPModule.VcxprojFilePath))
            {
                Guid ProjectGuid = Guid.NewGuid();
                KnownGuids.Add(ProjectGuid);
                ProjectGuids[CPPModule.VcxprojFilePath] = ProjectGuid;
            }
        }

        foreach (CSharpModule CSharpModule in Project.CSharpModules)
        {
            if (!ProjectGuids.ContainsKey(CSharpModule.FullPath))
            {
                Guid ProjectGuid = Guid.NewGuid();
                KnownGuids.Add(ProjectGuid);
                ProjectGuids[CSharpModule.FullPath] = ProjectGuid;
            }
        }

        foreach (CPPModule Module in Project.CPPModules)
            GenerateVcxproj(Module);


        StringWriter SWriter = new StringWriter();

        // Begin
        SWriter.WriteLine();
        SWriter.WriteLine($"Microsoft Visual Studio Solution File, Format Version {SolutionFileVersion}");
        SWriter.WriteLine($"# Visual Studio Version {SolutionFileVisualStudioMajorVersion}");
        SWriter.WriteLine($"VisualStudioVersion = {SolutionFileVisualStudioVersion}");
        SWriter.WriteLine($"MinimumVisualStudioVersion = {MinimumVisualStudioVersion}");

        // Virtual Folders
        {
            void WriteVirtualFolderProjects(List<ProjectVirtualFolder> VirtualFolders)
            {
                foreach (ProjectVirtualFolder VirtualFolder in VirtualFolders)
                {
                    Guid VirtualFolderGuid = KnownVirtualFolderGuids[VirtualFolder.FullPath];
                    SWriter.WriteLine($"Project(\"{{{Utils.GetModuleCategaryGuid(ModuleCategory.Folder)}}}\") = \"{VirtualFolder.Name}\", \"{VirtualFolder.Name}\", \"{VirtualFolderGuid.ToString("B").ToUpper()}\"");
                    SWriter.WriteLine("EndProject");
                    WriteVirtualFolderProjects(VirtualFolder.Children);
                }
            }

            WriteVirtualFolderProjects(Project.VirtualRoot.Children);
        }

        // Projects
        foreach (IProjectModule Module in Project.Modules)
        {
            string ModuleRelativePath = Path.GetRelativePath(Project.ProjectRoot, Module.FullPath);
            SWriter.WriteLine($"Project(\"{{{Utils.GetModuleCategaryGuid(Module.Category)}}}\") = \"{Module.ModuleName}\", \"{ModuleRelativePath}\", \"{ProjectGuids[Module.FullPath].ToString("B").ToUpper()}\"");
            SWriter.WriteLine("EndProject");
        }

        // Global Section
        SWriter.WriteLine("Global");
        {
            // SolutionConfigurationPlatforms
            SWriter.WriteLine("\tGlobalSection(SolutionConfigurationPlatforms) = preSolution");
            foreach (string Configuration in Project.Configurations)
            {
                foreach (string Platform in Project.Platforms)
                {
                    SWriter.WriteLine($"\t\t{Configuration}|{Platform} = {Configuration}|{Platform}");
                }
            }
            SWriter.WriteLine("\tEndGlobalSection");
        }
        {
            // SolutionConfigurationPlatforms
            SWriter.WriteLine("\tGlobalSection(ProjectConfigurationPlatforms) = postSolution");
            foreach (CPPModule Module in Project.CPPModules)
            {
                //SWriter.WriteLine($"\t\t{Module.ProjectGuid.ToString("B").ToUpper()}.{Project.ActiveConfiguration}|{Project.ActivePlatform}.ActiveCfg = {Project.ActiveConfiguration}|{Project.ActivePlatform}");
                foreach (string Configuration in Project.Configurations)
                {
                    foreach (string Platform in Project.Platforms)
                    {
                        SWriter.WriteLine($"\t\t{ProjectGuids[Module.FullPath].ToString("B").ToUpper()}.{Configuration}|{Platform}.ActiveCfg = {Configuration}|{Platform}");
                        SWriter.WriteLine($"\t\t{ProjectGuids[Module.FullPath].ToString("B").ToUpper()}.{Configuration}|{Platform}.Build.0 = {Configuration}|{Platform}");
                    }
                }
            }
            foreach (CSharpModule Module in Project.CSharpModules)
            {
                //SWriter.WriteLine($"\t\t{Module.ProjectGuid.ToString("B").ToUpper()}.{Project.ActiveConfiguration}|{Project.ActivePlatform}.ActiveCfg = {Project.ActiveConfiguration}|{Project.ActivePlatform}");
                foreach (string Configuration in Project.Configurations)
                {
                    foreach (string Platform in Project.Platforms)
                    {
                        SWriter.WriteLine($"\t\t{ProjectGuids[Module.FullPath].ToString("B").ToUpper()}.{Configuration}|{Platform}.ActiveCfg = {Configuration}|Any CPU");
                        SWriter.WriteLine($"\t\t{ProjectGuids[Module.FullPath].ToString("B").ToUpper()}.{Configuration}|{Platform}.Build.0 = {Configuration}|Any CPU");
                    }
                }
            }
            SWriter.WriteLine("\tEndGlobalSection");
        }
        {
            //SolutionProperties
            SWriter.WriteLine("\tGlobalSection(SolutionProperties) = preSolution");
            SWriter.WriteLine("\t\tHideSolutionNode = FALSE");
            SWriter.WriteLine("\tEndGlobalSection");
        }
        SWriter.WriteLine("EndGlobal");

        // NestedProjects
        {
            SWriter.WriteLine("\tGlobalSection(NestedProjects) = preSolution");
            void WriteVirtualFolderNestedFolders(List<ProjectVirtualFolder> VirtualFolders)
            {
                foreach (ProjectVirtualFolder VirtualFolder in VirtualFolders)
                {
                    if (!VirtualFolder.Parent!.IsRoot)
                        SWriter.WriteLine($"\t\t{KnownVirtualFolderGuids[VirtualFolder.FullPath].ToString("B").ToUpper()} = {KnownVirtualFolderGuids[VirtualFolder.Parent.FullPath].ToString("B").ToUpper()}");
                    WriteVirtualFolderNestedFolders(VirtualFolder.Children);
                }
            }
            WriteVirtualFolderNestedFolders(Project.VirtualRoot.Children);

            void WriteVirtualFolderNestedProjects(List<ProjectVirtualFolder> VirtualFolders)
            {
                foreach (ProjectVirtualFolder VirtualFolder in VirtualFolders)
                {
                    foreach (IProjectModule Module in VirtualFolder.Modules)
                    {
                        Guid ProjectGuid = ProjectGuids[Module.FullPath];
                        SWriter.WriteLine($"\t\t{ProjectGuids[Module.FullPath].ToString("B").ToUpper()} = {KnownVirtualFolderGuids[VirtualFolder.FullPath].ToString("B").ToUpper()}");
                    }
                    WriteVirtualFolderNestedProjects(VirtualFolder.Children);
                }
            }
            WriteVirtualFolderNestedProjects(Project.VirtualRoot.Children);
            SWriter.WriteLine("\tEndGlobalSection");
        }

        // End
        SWriter.Close();

        string SolutionFileContent = SWriter.ToString();
        if (!string.IsNullOrEmpty(MD5StringExists))
        {
            string MD5StringThis = Convert.ToHexString(MD5Computer.ComputeHash(Encoding.ASCII.GetBytes(SolutionFileContent)));
            if (MD5StringThis.Equals(MD5StringExists))
                return true;
        }

        File.WriteAllText(SolutionFilePath, SolutionFileContent, Encoding.UTF8);
        return true;
    }
}