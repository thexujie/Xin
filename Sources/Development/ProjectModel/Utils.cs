using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

namespace ProjectModel
{
    public class Utils
    {
        public static string GetModuleCategaryGuid(ModuleCategory Category)
        {
            switch (Category)
            {
            case ModuleCategory.CSharp:
                return "FAE04EC0-301F-11D3-BF4B-00C04F79EFBC";
            case ModuleCategory.Net:
                return "9A19103F-16F7-4668-BE54-9A1E7A4F7556";
            case ModuleCategory.CPP:
                return "8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942";
            case ModuleCategory.Folder:
                return "2150E333-8FDC-42A3-9474-1A3956D46DE8";
            default:
            case ModuleCategory.None:
                return "00000000-0000-0000-0000-000000000000";
            }
        }

        public static string GetModuleTypeGuid(ModuleType Type)
        {
            switch (Type)
            {
            case ModuleType.DynamicLibrary:
                return "D9008B0C-E9BD-4A8E-A353-08CEF2D6337C";
            case ModuleType.StaticLibrary:
                return "BF3EFEB1-011F-4125-BDAB-C027A49895C5";
            case ModuleType.Application:
                return "6A1218AF-5BCB-495F-B935-5B645D8DC22D";
            default:
            case ModuleType.None:
                return "00000000-0000-0000-0000-000000000000";
            }
        }
    }
}
