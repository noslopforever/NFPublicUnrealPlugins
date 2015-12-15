//Disclaimer: This is preliminary software and/or hardware and APIs are preliminary and subject to change

namespace UnrealBuildTool.Rules
{
    public class RESkyEffects : ModuleRules
    {
        public RESkyEffects(TargetInfo Target)
        {
            PublicIncludePaths.AddRange(
                new string[] {
					// ... add public include paths required here ...
				}
                );

            PrivateIncludePaths.AddRange(
                new string[] {
					"RESkyEffects/Private",
					// ... add other private include paths required here ...
				}
                );

            PublicDependencyModuleNames.AddRange(
                new string[]
				{
                    "Core",
					"CoreUObject",
                    "Engine",
                    "RenderCore",
                    "ShaderCore",
                    "RHI",
					// ... add other public dependencies that you statically link with here ...
					"Slate",
                    "Renderer",
                    "RESkyEngine",
				}
                );

            PrivateDependencyModuleNames.AddRange(
                new string[]
				{
					// ... add private dependencies that you statically link with here ...
                    "PhysXFormats",
                    //"TargetPlatform",
				}
                );

            DynamicallyLoadedModuleNames.AddRange(
                new string[]
				{
					// ... add any modules that your module loads dynamically here ...
				}
                );

            if (UEBuildConfiguration.bBuildEditor == true)
            {
                PrivateDependencyModuleNames.Add("UnrealEd");
            }

            PublicDependencyModuleNames.Add("libnoise");
            //AddThirdPartyPrivateStaticDependencies(Target, "libnoise");
        }
    }
}