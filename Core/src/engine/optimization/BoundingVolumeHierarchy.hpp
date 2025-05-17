/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** BoundingVolumeHierarchy.hpp
*/

#pragma once

#include <algorithm>
#include <functional>

#include "engine/Scene.hpp"
#include "engine/debug/DebugDiffuse.hpp"
#include "MATH/Constants.hpp"
#include "RACIST/IObject.hpp"

namespace raytracer::engine
{
    class Scene;

    class BoundingVolumeHierarchyNode : public IObject
    {
        public:
            explicit BoundingVolumeHierarchyNode(Scene& scene)
                : BoundingVolumeHierarchyNode(scene.getObjects(), 0, scene.getObjects().size()) {}
            BoundingVolumeHierarchyNode(std::vector<IObject*> objects, const std::size_t start, const std::size_t end)
            {
                std::function<bool(const IObject*, const IObject*)> compareFunction;
                switch (math::random_int(0, 2)) {
                    case 0: compareFunction = boxCompareX; break;
                    case 1: compareFunction = boxCompareY; break;
                    case 2: compareFunction = boxCompareZ; break;
                    default:
                        throw std::exception();
                }

                switch (end - start)
                {
                    case 1:
                        this->_leftObject = objects.at(start);
                        this->_rightObject = this->_leftObject;
                        break;
                    case 2:
                        this->_leftObject = objects.at(start);
                        this->_rightObject = objects.at(start + 1);
                        break;
                    default:
                        {
                            std::sort(std::begin(objects) + start, std::begin(objects) + end, compareFunction);

                            const auto mid = start + (end - start) / 2;
                            this->_leftBVHNode = std::make_unique<BoundingVolumeHierarchyNode>(objects, start, mid);
                            this->_rightBVHNode = std::make_unique<BoundingVolumeHierarchyNode>(objects, mid, end);
                            this->_leftObject = this->_leftBVHNode.get();
                            this->_rightObject = this->_rightBVHNode.get();
                        }
                }

                if (DEBUG_BOUNDARIES)
                    this->_debugMatPtr = std::make_unique<materials::DebugDiffuse>(20, 200, 20);
                this->_boundingBox = AABB(this->_leftObject->getBoundingDox(), this->_rightObject->getBoundingDox());
            }

            bool hit(const Ray& ray, const math::Interval& ray_t, HitRecord& record) const override
            {
                if (!_boundingBox.hit(ray, ray_t))
                    return false;

                const bool hitLeft = this->_leftObject->hit(ray, ray_t, record);
                const bool hitRight =
                    this->_rightObject->hit(
                        ray,
                        math::Interval{ray_t.min, (hitLeft ? record.t : ray_t.max)},
                        record
                    );


                if (DEBUG_BOUNDARIES) {
                    record.material = this->_debugMatPtr.get();
                    record.normal = math::Vec3<double>{1.0, 1.0, 1.0};
                    record.point = math::Point3D{1, 1, 1};
                    record.point_outward = true;
                    record.t = 1.0;
                    return true;
                }

                return hitLeft || hitRight;
            }
            [[nodiscard]] AABB getBoundingDox() const override
            {
                return this->_boundingBox;
            }
            void setMaterial(std::unique_ptr<materials::IMaterial>& material) override
            {
                throw std::exception();
            }

        private:
            static constexpr bool DEBUG_BOUNDARIES = false;

            std::unique_ptr<IObject> _leftBVHNode;
            std::unique_ptr<IObject> _rightBVHNode;
            IObject* _leftObject;
            IObject* _rightObject;
            AABB _boundingBox;
            std::unique_ptr<materials::IMaterial> _debugMatPtr;

            static bool boxCompare(const IObject* a, const IObject* b, const int axisIdx)
                { return a->getBoundingDox().axisInterval(axisIdx).min < b->getBoundingDox().axisInterval(axisIdx).min; }
            static bool boxCompareX(const IObject* a, const IObject* b) { return boxCompare(a, b, 0); }
            static bool boxCompareY(const IObject* a, const IObject* b) { return boxCompare(a, b, 1); }
            static bool boxCompareZ(const IObject* a, const IObject* b) { return boxCompare(a, b, 2); }
    };

    typedef BoundingVolumeHierarchyNode BVHNode;
}
