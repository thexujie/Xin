// See https://aka.ms/new-console-template for more information

using ProjectModel;
using ProjectGenerator.VisualStudio;

string[] CommandLineArgs = Environment.GetCommandLineArgs();

string ProjectPath = Directory.GetCurrentDirectory();

if (string.IsNullOrEmpty(ProjectPath) || !Directory.Exists(ProjectPath))
    return;

string ProjectFilePath = Path.Combine(ProjectPath, Path.GetFileName(ProjectPath) + ".xin");
Console.WriteLine($"Parsing... \"{ProjectFilePath}\"");
Project? Project = Project.GenerateFromXinFile(ProjectFilePath);
if (Project != null)
{
    SolutionGenerator Generator = new SolutionGenerator();
    Generator.GenerateSolution(Project);
}

Console.WriteLine("Finish!");
Console.ReadKey();
