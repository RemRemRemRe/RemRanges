// Copyright RemRemRemRe. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class transrangers : ModuleRules
{
	public transrangers(ReadOnlyTargetRules target) : base(target)
	{
		Type = ModuleType.External;

		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "transrangers/include"));
	}
}
