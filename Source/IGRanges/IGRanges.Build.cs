// Copyright Ian Good

using UnrealBuildTool;

public class IGRanges : ModuleRules
{
	public IGRanges(ReadOnlyTargetRules Target) : base(Target)
	{
		UnsafeTypeCastWarningLevel = WarningLevel.Warning;

		bEnableNonInlinedGenCppWarnings = true;
		bUseUnity = false;

		PublicDependencyModuleNames.AddRange(
			[
				"Core",
				"CoreUObject",

				"RemRanges",
			]
		);
	}
}
