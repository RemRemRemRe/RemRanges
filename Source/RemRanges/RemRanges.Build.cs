// Copyright RemRemRemRe. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class RemRanges : ModuleRules
{
	public RemRanges(ReadOnlyTargetRules target) : base(target)
	{
		ShadowVariableWarningLevel = WarningLevel.Error;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		CppStandard = CppStandardVersion.EngineDefault;
		UnsafeTypeCastWarningLevel = WarningLevel.Warning;

		bEnableNonInlinedGenCppWarnings = true;
		bUseUnity = false;

		PrivateDependencyModuleNames.AddRange(
			new[]
			{
				"Core",
				"CoreUObject",
				"Engine",

				"DeveloperSettings",
				"RemCommon",
			}
		);
	}
}
