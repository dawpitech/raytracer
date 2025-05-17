/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** CameraMan.hpp
*/

#pragma once

#include <iostream>

#include "Camera.hpp"

namespace raytracer::engine
{
    class CameraMan
    {
        public:
            class CameraManData
            {
                public:
                    const WorldConfiguration& worldConfiguration;
                    const Scene& scene;
                    graphics::IRenderer& renderer;
                    const bool multithreading;
                    const unsigned int threadsCount;
            };

            CameraMan() : CameraMan(1.0, 400, false) {}
            CameraMan(const double aspect_ratio, const int image_width, const bool VR)
                : _isVR(VR)
            {
                if (!VR) {
                    this->_primaryCamera = std::make_unique<Camera>(aspect_ratio, image_width);
                } else {
                    this->_primaryCamera = std::make_unique<Camera>(1.0, image_width);
                    this->_secondaryCamera = std::make_unique<Camera>(1.0, image_width);
                    _VRCanva = std::make_unique<graphics::Canva>(image_width * 2, image_width);
                }
            }
            ~CameraMan() = default;

            void switchToVR()
            {
                this->_primaryCamera = std::make_unique<Camera>(1.0, 400);
                this->_secondaryCamera = std::make_unique<Camera>(1.0, 400);
                _VRCanva = std::make_unique<graphics::Canva>(400 * 2, 400);
                this->_isVR = true;
            }
            void updateCanvaSize(const int imageWidth)
            {
                _VRCanva = std::make_unique<graphics::Canva>(imageWidth * 2 + 2, imageWidth + 2);
                for (int y = 0; y < imageWidth + 2; y++)
                    for (int x = 0; x < imageWidth * 2 + 2; x++)
                        this->_VRCanva->setPixelColor(x, y, graphics::Color{});
            }
            [[nodiscard]] std::vector<Camera*> getCameras() const
            {
                if (_isVR)
                    return {this->_primaryCamera.get(), this->_secondaryCamera.get()};
                return {this->_primaryCamera.get()};
            }
            [[nodiscard]] int render(const CameraManData& data) const
            {
                if (!_isVR) {
                    if (data.multithreading)
                        return this->_primaryCamera->render(data.worldConfiguration, data.scene, data.renderer, data.threadsCount), 0;
                    return this->_primaryCamera->renderNoThread(data.worldConfiguration, data.scene, data.renderer);
                } else {
                    if (data.multithreading)
                        std::cout << "[INFO] Multi-threading was automatically disable for VR rendering!" << std::endl;
                    const auto& leftEye = this->_primaryCamera->renderRaw(data.worldConfiguration, data.scene, data.renderer);
                    const auto& rightEye = this->_secondaryCamera->renderRaw(data.worldConfiguration, data.scene, data.renderer);
                    std::cout << "Working here ?" << std::endl;
                    std::cout << this->_VRCanva->getWidth() << "/" << this->_VRCanva->getHeight() << std::endl;
                    for (int y = 1; y < leftEye.getHeight(); y++) {
                        for (int x = 0; x < leftEye.getWidth(); x++)
                            this->_VRCanva->setPixelColor(x, y, leftEye.getPixelColor(x, y));
                        for (int x = 1; x < rightEye.getWidth(); x++)
                            this->_VRCanva->setPixelColor(x + leftEye.getWidth(), y, rightEye.getPixelColor(x, y));
                    }
                    std::cout << "But crash before?" << std::endl;
                    data.renderer.renderCanva(*this->_VRCanva);
                    std::cout << "Is it done?" << std::endl;
                    return 0;
                }
            }
            void updateRenderingConfig() const
            {
                this->_primaryCamera->updateRenderingConfig();
                if (_isVR)
                    this->_secondaryCamera->updateRenderingConfig();
            }

        private:
            bool _isVR;
            std::unique_ptr<graphics::Canva> _VRCanva;
            std::unique_ptr<Camera> _primaryCamera;
            std::unique_ptr<Camera> _secondaryCamera;
    };
}
