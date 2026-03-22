// Copyright RemRemRemRe. All Rights Reserved.

using System.IO;
using UnrealBuildTool;
using Rem.BuildRule;

public class RemRanges : ModuleRules
{
	public RemRanges(ReadOnlyTargetRules target) : base(target)
	{
        RemSharedModuleRules.Apply(this);

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
