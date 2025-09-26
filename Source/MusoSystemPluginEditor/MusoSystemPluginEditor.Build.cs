using UnrealBuildTool;

public class MusoSystemPluginEditor: ModuleRules
{
    public MusoSystemPluginEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PrivateDependencyModuleNames.AddRange(
			new string[] {
				"Core", 
				"CoreUObject", 
				"Engine",
				"MusoSystemPlugin",
				"UnrealEd"
            }
		);
		PublicDependencyModuleNames.AddRange(
			new string[] {
				"AssetDefinition",
                "MusoSystemPlugin",
            }
        );
    }
}
