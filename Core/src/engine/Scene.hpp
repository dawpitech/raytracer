/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Scene.hpp
*/

#pragma once

#include <memory>
#include <vector>

#include "RACIST/AxisAlignBoundingBox.hpp"
#include "RACIST/HitRecord.hpp"
#include "RACIST/IObject.hpp"

namespace raytracer::engine
{
    class Scene : public IObject
    {
        public:
            Scene() = default;
            ~Scene() override = default;

            void clear() { this->_objects.clear(); }
            void add(std::unique_ptr<IObject>& object)
            {
                this->_boundingBox = AABB(this->_boundingBox, object->getBoundingDox());
                this->_objects.push_back(std::move(object));
            }

            bool hit(const Ray& ray,
                const math::Interval& ray_t,
                HitRecord& record) const override
            {
                HitRecord tmp_record;
                bool hit_anything = false;
                auto closest_so_far = ray_t.max;
                for (const auto& object: this->_objects) {
                    if (object->hit(ray, math::Interval{ray_t.min, closest_so_far}, tmp_record)) {
                        hit_anything = true;
                        closest_so_far = tmp_record.t;
                        record = tmp_record;
                    }
                }

                return hit_anything;
            }

            void setMaterial(std::unique_ptr<materials::IMaterial>& material) override
            {
                throw std::exception();
            }
            [[nodiscard]] AABB getBoundingDox() const override
            {
                return this->_boundingBox;
            }
            [[nodiscard]] std::vector<IObject*> getObjects()
            {
                std::vector<IObject*> _objectsRef;
                for (auto& obj : this->_objects)
                    _objectsRef.push_back(obj.get());

                return _objectsRef;
            }

        private:
            std::vector<std::unique_ptr<IObject>> _objects;
            AABB _boundingBox;
    };
}
