#include "Main.h"
#include "Platform/MessageBox.hpp"
#include "Platform/ApplicationSingleInstance.hpp"
#include "Debugger/Logger.hpp"
#include "SteamAPI/SteamAPI.hpp"
#include "Utility/Utility.h"
#include "AppFrame.h"
#include "RuntimeCheck.hpp"
#include "core/Configuration.hpp"

int luastg::sub::main() {
#ifdef _DEBUG
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
	// _CrtSetBreakAlloc(5351);
#endif

	// STAGE 1: load application configurations

	auto& config_loader = core::ConfigurationLoader::getInstance();
	if (!config_loader.loadFromFile("config.json")) {
		Platform::MessageBox::Error(LUASTG_INFO, config_loader.getFormattedMessage());
		return EXIT_FAILURE;
	}

	// STAGE 2: configure single instance

	Platform::ApplicationSingleInstance single_instance(LUASTG_INFO);
	if (auto const& config_app = config_loader.getApplication(); config_app.isSingleInstance()) {
		single_instance.Initialize(config_app.getUuid());
	}

	// STAGE 3: initialize COM

	LuaSTGPlus::CoInitializeScope com_runtime;
	if (!com_runtime())
	{
		Platform::MessageBox::Error(LUASTG_INFO,
			"引擎初始化失败。\n"
			"未能正常初始化COM组件库，请尝试重新启动此应用程序。");
		return EXIT_FAILURE;
	}

	// STAGE 4: check runtime

	if (!LuaSTG::CheckUserRuntime()) {
		return EXIT_FAILURE;
	}

	// STAGE 5: start

	LuaSTG::Debugger::Logger::create();

	int result = EXIT_SUCCESS;
	if (LuaSTG::SteamAPI::Init())
	{
		if (LAPP.Init())
		{
			LAPP.Run();
			result = EXIT_SUCCESS;
		}
		else
		{
			Platform::MessageBox::Error(LUASTG_INFO,
				"引擎初始化失败。\n"
				"查看日志文件（engine.log，可以用记事本打开）可以获得更多信息。\n"
				"请尝试重新启动此应用程序，或者联系开发人员。");
			result = EXIT_FAILURE;
		}
		LAPP.Shutdown();
		LuaSTG::SteamAPI::Shutdown();
	}
	else
	{
		result = EXIT_FAILURE;
	}

	LuaSTG::Debugger::Logger::destroy();

	return result;
}
