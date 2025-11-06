#include "Triangle.hpp"

// Constructor for the Triangle class
Triangle::Triangle(const Vector3& vertex0, const Vector3& vertex1, const Vector3& vertex2, const Material& material)
    : vertex0(vertex0), vertex1(vertex1), vertex2(vertex2), material(material) {} // Initializes the triangle's vertices and material

// Ray-triangle intersection test using the Möller-Trumbore algorithm
bool Triangle::intersect(const Ray& ray, float tMin, float tMax, HitRecord& hitRecord) const {
    // Get triangle edges
    Vector3 edge1 = vertex1 - vertex0; // Edge from vertex0 to vertex1
    Vector3 edge2 = vertex2 - vertex0; // Edge from vertex0 to vertex2

    // Calculate determinant
    Vector3 pvec = ray.direction.Cross(edge2); // pvec is the cross product of the ray's direction and edge2
    float determinant = edge1 * pvec; // determinant is the dot product of edge1 and pvec

    // If the determinant is close to zero, the ray is parallel to the triangle plane
    if (determinant > -1e-6 && determinant < 1e-6) {
        return false; // No intersection
    }

    float invDeterminant = 1.0 / determinant; // Inverse of the determinant
    Vector3 tvec = ray.origin - vertex0; // tvec is the vector from vertex0 to the ray's origin
    float barycentricU = invDeterminant * (tvec * pvec); // u is the barycentric coordinate

    // Check if the intersection point is outside the triangle
    if (barycentricU < 0.0 || barycentricU > 1.0) {
        return false; // No intersection
    }

    Vector3 qvec = tvec.Cross(edge1); // qvec is the cross product of tvec and edge1
    float barycentricV = invDeterminant * (ray.direction * qvec); // v is the barycentric coordinate

    // Check if the intersection point is outside the triangle
    if (barycentricV < 0.0 || barycentricU + barycentricV > 1.0) {
        return false; // No intersection
    }

    // Calculate t, the distance from the ray's origin to the intersection point
    float intersectionDistance = invDeterminant * (edge2 * qvec);

    // Check if the intersection is within the valid range [tMin, tMax]
    if (intersectionDistance > tMin && intersectionDistance < tMax) {
        hitRecord.t = intersectionDistance; // Set the intersection distance
        hitRecord.point = ray.origin + ray.direction * intersectionDistance; // Calculate the intersection point
        hitRecord.normal = edge1.Cross(edge2).Normalized(); // Calculate the triangle's normal vector
        hitRecord.material = const_cast<Material*>(&material); // Set the material of the hit record
        return true; // Intersection found
    }

    return false; // No intersection within the valid range
}
