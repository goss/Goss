#include "gepch.h"
#include "RenderCommand.h"

#include "Core.h"
#include "RendererAPI.h"

namespace Goss
{
	Scope<RendererAPI> RenderCommand::rendererAPI = RendererAPI::Create();
}
