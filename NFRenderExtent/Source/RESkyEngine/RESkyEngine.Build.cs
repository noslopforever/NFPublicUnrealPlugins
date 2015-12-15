//Disclaimer: This is preliminary software and/or hardware and APIs are preliminary and subject to change

namespace UnrealBuildTool.Rules
{
    public class RESkyEngine : ModuleRules
    {
        public RESkyEngine(TargetInfo Target)
        {
            PublicIncludePaths.AddRange(
                new string[] {
					// ... add public include paths required here ...
				}
                );

            PrivateIncludePaths.AddRange(
                new string[] {
					"RECloudEngine/Private",
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
                    "RHI"
				}
                );

            PrivateDependencyModuleNames.AddRange(
                new string[] {
				    "Renderer"
			    }
                );

            // TODO impl ¡Ÿ ±
            PublicDependencyModuleNames.Add("libnoise");

        }
    }
}