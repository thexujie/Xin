using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ProjectModel
{
    [AttributeUsage(AttributeTargets.All, Inherited = true)]
    public sealed class ModuleTypeGUIDAttribute : Attribute
    {
        public Guid GUID
        {
            get; set;
        }
        public ModuleTypeGUIDAttribute(Guid GUID)
        {
            this.GUID = GUID;
        }
        public ModuleTypeGUIDAttribute(string GUID)
        {
            this.GUID = Guid.Parse(GUID);
        }
    }

    public enum ModuleType
    {
        None,
        DynamicLibrary,
        StaticLibrary,
        Application,
        Makefile
    }

    public enum ModuleSubType
    {
        None,
        NativeUnitTestProject, // C++ Unit Test
    }

    [AttributeUsage(AttributeTargets.All)]
    public sealed class VSModuleCategoryGUIDAttribute : Attribute
    {
        public Guid GUID
        {
            get; set;
        }
        public VSModuleCategoryGUIDAttribute(Guid GUID)
        {
            this.GUID = GUID;
        }
        public VSModuleCategoryGUIDAttribute(string GUID)
        {
            this.GUID = Guid.Parse(GUID);
        }
    }

    public enum ModuleCategory
    {
        None,
        CSharp,
        CPP,
        Folder,
        Net,
    }

    public enum ModuleConfiguration
    {
        Default,
        Debug,
        Release,
    }

    public enum ModulePlatform
    {
        Default,
        x64,
        x86,
    }

    public class ModuleConditionalRules
    {
        public List<string> PublicAdditionalLibraries = new();

        public List<string> PrivateIncludePaths = new List<string>();
        public List<string> PrivateLibraryPaths = new List<string>();
        public List<string> PrivateLibraries = new();
    }

    [DebuggerDisplay("{Name} {Type} Module")]
    public class IModule
    {
        public string Name { get; set; } = string.Empty;

        /// <summary>
        /// Path of Module.cs
        /// </summary>
        public string ModuleFilePath { get; set; } = string.Empty;

        public string VirtualFolder { get; set; } = string.Empty;

        public ModuleType Type { get; set; } = ModuleType.None;
        public ModuleSubType SubType { get; set; } = ModuleSubType.None;

        public string ModuleFileName => Path.GetFileName(ModuleFilePath);

        public string PCHFile = "PCH.cpp";

        public List<string> Files = new List<string>();
        public List<string> AdditionalFiles = new List<string>();

        public List<string> PrivateDependencyModules = new List<string>();
        public List<string> PublicDependencyModules = new List<string>();

        public List<string> PrivateDefines = new List<string>();
        public List<string> PublicDefines = new List<string>();

        public Dictionary<ModuleCondition, ModuleConditionalRules> ConditionalRules = new()
        {
            { ModuleCondition.Default, new ModuleConditionalRules() },
            { ModuleCondition.Debug, new ModuleConditionalRules() },
            { ModuleCondition.Release, new ModuleConditionalRules() },
            { ModuleCondition.x86, new ModuleConditionalRules() },
            { ModuleCondition.x64, new ModuleConditionalRules() },
            { ModuleCondition.Debug_x86, new ModuleConditionalRules() },
            { ModuleCondition.Debug_x64, new ModuleConditionalRules() },
            { ModuleCondition.Release_x86, new ModuleConditionalRules() },
            { ModuleCondition.Release_x64, new ModuleConditionalRules() },
        };
        
        public ModuleConditionalRules Debug_x64 => ConditionalRules[ModuleCondition.Debug_x64];
        public ModuleConditionalRules Release_x64 => ConditionalRules[ModuleCondition.Release_x64];

        public List<string> PrivateIncludePaths => ConditionalRules[ModuleCondition.Default].PrivateIncludePaths;
        public List<string> PrivateLibraryPaths => ConditionalRules[ModuleCondition.Default].PrivateLibraryPaths;
        public List<string> PrivateLibraries => ConditionalRules[ModuleCondition.Default].PrivateLibraries;

        public IModule()
        {
            //Category = ModuleCategory.CPP;
        }
    }
}
