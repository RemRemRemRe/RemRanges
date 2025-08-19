// Copyright RemRemRemRe. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class RemRanges : ModuleRules
{
	public RemRanges(ReadOnlyTargetRules target) : base(target)
	{
		CppCompileWarningSettings.ShadowVariableWarningLevel = WarningLevel.Error;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		CppStandard = CppStandardVersion.EngineDefault;
		CppCompileWarningSettings.UnsafeTypeCastWarningLevel = WarningLevel.Warning;

		CppCompileWarningSettings.NonInlinedGenCppWarningLevel = WarningLevel.Warning;
		bUseUnity = false;

		PrivateDependencyModuleNames.AddRange(
			[
				"Core",
				"CoreUObject",
				"Engine",

				"DeveloperSettings",
				"RemCommon",
				"transrangers",
			]
		);
	}
}
