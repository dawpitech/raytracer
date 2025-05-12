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

void raytracer::Raytracer::run()
{
    const std::string rendererName = "PPM";
    try {
        this->_renderer = graphics::RendererFactory::createRenderer(rendererName);
    } catch (graphics::RendererFactory::UnknownRendererException&) {
        std::cerr << "[ERR!] No valid renderer found with name: " << rendererName << std::endl;
        return;
    }

    std::clog << "[INFO] Watch mode: " << (this->_config.watchingConfig ? "enabled" : "disabled") << std::endl;

    this->_camera.render(this->_worldConfig, _world, *this->_renderer);

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
                this->_camera.render(this->_worldConfig, this->_world, *this->_renderer);
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
    }
    try {
        this->_camera.updateRenderingConfig();
    } catch (engine::Camera::CameraException&) {
        std::cerr << "[ERR!] Given configuration couldn't be rendered, are requirements matched ?" << std::endl;
        return -1;
    }
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
    for (int i = 1; i < argc && argv != nullptr; i++) {
        const auto arg = std::string(argv[i]);
        if (arg.at(0) == '-') {
            const auto& flag = arg.substr(1);
            if (flag == "-help" || flag == "h")
                return printHelp(), 1;
            if (flag == "w" || flag == "-watch")
                this->_config.watchingConfig = true;
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
    std::cout << "USAGE: ./raytracer <SCENE_FILE> [-w|--watch] [-h|--help]"
        << "\n\tSCENE_FILE: scene configuration" << std::endl;
}
