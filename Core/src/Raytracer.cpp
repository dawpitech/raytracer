/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Raytracer.cpp
*/

#include <iostream>
#include <thread>

#include "Raytracer.hpp"
#include "engine/renderers/RendererFactory.hpp"
#include "parser/MasterParser.hpp"
#include "plugins/ModuleLoader.hpp"
#include "plugins/SafeDirectoryLister.hpp"
#include "utils/FileWatcher.hpp"
#include "engine/optimization/BoundingVolumeHierarchy.hpp"

int raytracer::Raytracer::render() const
{
    if (this->_config.multithreading)
        return this->_camera.render(this->_worldConfig, *this->_optimizedWorld, *this->_renderer, this->_config.threadCount), 0;
    else
        return this->_camera.renderNoThread(this->_worldConfig, *this->_optimizedWorld, *this->_renderer);
}


void raytracer::Raytracer::run()
{
    try {
        this->_renderer = graphics::RendererFactory::createRenderer(this->_config.renderer);
    } catch (graphics::RendererFactory::UnknownRendererException&) {
        std::cerr << "[ERR!] No valid renderer found with name: " << this->_config.renderer << std::endl;
        return;
    }

    std::clog << "[INFO] Rendering using " << this->_renderer->getName() << std::endl;
    std::clog << "[INFO] Watch mode: " << (this->_config.watchingConfig ? "enabled" : "disabled") << std::endl;

    if (this->render() != 0)
        return;

    if (this->_config.watchingConfig)
    {
        std::clog << "[TRACE] Waiting for scene configuration update..." << std::endl;
        while (this->_renderer->isInteractive() ? !this->_renderer->exitRequested() : true) {
            if (const auto editTime = utils::FileWatcher::getLastEditTime(this->_config.sceneConfigurationFilePath);
                editTime != this->_configFileLastEditTime) {
                std::clog << "[TRACE] Scene configuration update occurred, rendering automatically" << std::endl;
                this->_configFileLastEditTime = editTime;
                this->_world.clear();
                this->parseSceneConfig();
                if (this->render() != 0)
                    return;
                std::clog << "[TRACE] Waiting for scene configuration update..." << std::endl;
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }
    } else {
        if (!this->_renderer->isInteractive())
            return;
        do {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        } while (!this->_renderer->exitRequested());
    }
}

void raytracer::Raytracer::loadPlugins()
{
    const SafeDirectoryLister lister{"plugins", false};

    try {
        while (true) {
            const std::string pluginName = lister.get();
            auto handle = SafeDL::open("plugins/" + pluginName, RTLD_LAZY);
            const auto version = ModuleLoader::getModuleVersion(handle);
            const auto type = ModuleLoader::getModuleType(handle);
            if (version >= generic::ModuleVersion::NOT_SUPPORTED || version == generic::ModuleVersion::UNKNOWN)
                return;
            switch (type) {
                case generic::ModuleType::PRIMITIVE:
                    {
                        const auto parser = ModuleLoader::retrieveObjectParser(handle);

                        for (const auto& supportedObject : parser->getSupportedObjects()) {
                            std::clog << "[TRACE] (" << pluginName << ") Plugin supporting object type '" << supportedObject << "'" << std::endl;
                            this->_objectsParser.insert(std::make_pair(supportedObject, ModuleLoader::retrieveObjectParser(handle)));
                        }

                        this->_pluginInventory.emplace_back(std::move(handle));
                        break;
                    }
                case generic::ModuleType::MATERIAL:
                    {
                        const auto parser = ModuleLoader::retrieveMaterialParser(handle);

                        for (const auto& supportedMaterial : parser->getSupportedMaterials()) {
                            std::clog << "[TRACE] (" << pluginName << ") Plugin supporting material type '" << supportedMaterial << "'" << std::endl;
                            this->_materialsParser.insert(std::make_pair(supportedMaterial, ModuleLoader::retrieveMaterialParser(handle)));
                        }

                        this->_pluginInventory.emplace_back(std::move(handle));
                        break;
                    }
                default:
                    throw std::exception();
            }
        }
    } catch (SafeDirectoryLister::NoMoreFileException& e) {}
}

int raytracer::Raytracer::parseSceneConfig()
{
    this->_configFileLastEditTime = utils::FileWatcher::getLastEditTime(this->_config.sceneConfigurationFilePath);
    try {
        parser::MasterParser::parseScene(this->_config.sceneConfigurationFilePath, *this);
    } catch (parser::MasterParser::ParserException& pe) {
        std::cerr << "[ERR!] " << pe.what() << std::endl;
        return -1;
    } catch (generic::IObjectParser::ObjectParserException &ope) {
        std::cerr << "[ERR!] (plugin reported:) " << ope.what() << std::endl;
        return -1;
    } catch (std::exception &e) {
        std::cerr << "[ERR!] Unknown error occurred, exiting..." << std::endl;
    }
    try {
        this->_camera.updateRenderingConfig();
    } catch (engine::Camera::CameraException&) {
        std::cerr << "[ERR!] Given configuration couldn't be rendered, are requirements matched ?" << std::endl;
        return -1;
    }
    std::unique_ptr<engine::IObject> optimizedObject = std::make_unique<engine::BVHNode>(this->_world);
    this->_optimizedWorld = std::make_unique<engine::Scene>();
    this->_optimizedWorld->add(optimizedObject);
    return 0;
}

const std::map<std::string, std::unique_ptr<raytracer::generic::IObjectParser>>& raytracer::Raytracer::getObjectsParser() const
{
    return this->_objectsParser;
}

const std::map<std::string, std::unique_ptr<raytracer::generic::IMaterialParser>>& raytracer::Raytracer::getMaterialsParser() const
{
    return this->_materialsParser;
}

raytracer::engine::Scene& raytracer::Raytracer::getMainScene()
{
    return this->_world;
}

raytracer::engine::Camera& raytracer::Raytracer::getMainCamera()
{
    return this->_camera;
}

raytracer::engine::WorldConfiguration& raytracer::Raytracer::getWorldConfig()
{
    return this->_worldConfig;
}

int raytracer::Raytracer::parseArgs(const int argc, const char** argv)
{
    bool shouldSkipArg = false;

    for (int i = 1; i < argc && argv != nullptr; i++) {
        if (shouldSkipArg) {
            shouldSkipArg = false;
            continue;
        }
        const auto arg = std::string(argv[i]);
        if (arg.at(0) == '-') {
            const auto& flag = arg.substr(1);
            if (flag == "-help" || flag == "h")
                return printHelp(), 1;
            if (flag == "w" || flag == "-watch")
                this->_config.watchingConfig = true;
            if (flag == "r" || flag == "-renderer") {
                if (i + 1 >= argc)
                    return std::cerr << "[ERR!] Renderer flag set without specifying a renderer" << std::endl, printHelp(), 84;
                this->_config.renderer = std::string(argv[i + 1]);
                shouldSkipArg = true;
            }
            if (flag == "t" || flag == "-threads") {
                if (i + 1 >= argc)
                    return std::cerr << "[ERR!] Threads flag set without specifying option" << std::endl, printHelp(), 84;
                const std::string option(argv[i + 1]);
                shouldSkipArg = true;
                if (option == "off") {
                    this->_config.multithreading = false;
                    this->_config.threadCount = 0;
                    continue;
                }
                if (option == "auto") {
                    this->_config.multithreading = true;
                    this->_config.threadCount = 0;
                    continue;
                }
                this->_config.multithreading = true;
                this->_config.threadCount = 0;
                try {
                    this->_config.threadCount = std::stoi(option);
                } catch (std::invalid_argument&) {}
                if (this->_config.threadCount <= 0 || this->_config.threadCount > 32)
                    return std::cerr << "[ERR!] Threads flag unknown (should be between 1-32)" << std::endl, printHelp(), 84;
            }
            continue;
        }
        if (!this->_config.sceneConfigurationFilePath.empty())
            return std::cerr << "[ERR!] Multiple scene configuration file given" << std::endl, printHelp(), 84;
        this->_config.sceneConfigurationFilePath = arg;
    }
    if (this->_config.sceneConfigurationFilePath.empty())
        return std::cerr << "[ERR!] No scene configuration file given" << std::endl, printHelp(), 84;
    return 0;
}

void raytracer::Raytracer::printHelp()
{
    std::cout << "USAGE: ./raytracer <SCENE_FILE> [-w|--watch] [-r|--renderer <SDL|PPM>] [-t|--threads <auto|off|1-32>] [-h|--help]"
        << "\n\t<SCENE_FILE>: scene configuration"
        << "\n\t[-w|--watch]: scene configuration auto-reload"
        << "\n\t[-r|--renderer <SDL|PPM>]: renderer selection (default: 'PPM')"
        << "\n\t[-t|--threads <auto|off|1-32>]: multi-threading selection -> auto/off/specific thread count (default: 'auto')"
        << "\n\t[-h|--help]: Show this help page" << std::endl;
}
