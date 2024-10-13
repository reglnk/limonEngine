//
// Created by engin on 14/01/2024.
//

#ifndef IMGUIREQUEST_H
#define IMGUIREQUEST_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct ImGuiRequest {
     glm::mat4& perspectiveCameraMatrix;
    const glm::mat4 orthogonalCameraMatrix = glm::lookAt(glm::vec3(0, 0, 1), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    const glm::mat4& perspectiveMatrix;
    const glm::mat4& orthogonalMatrix;

    const uint32_t& screenHeight;
    const uint32_t& screenWidth;

    LimonAPI* limonAPI = nullptr;

    ImGuiRequest(const glm::mat4 &perspectiveCameraMatrix, const glm::mat4 &perspectiveMatrix,
                 const glm::mat4 &orthogonalMatrix, const uint32_t &screenHeight, const uint32_t &screenWidth, LimonAPI* limonAPI)
            : perspectiveCameraMatrix(perspectiveCameraMatrix), perspectiveMatrix(perspectiveMatrix),
              orthogonalMatrix(orthogonalMatrix), screenHeight(screenHeight), screenWidth(screenWidth), limonAPI(limonAPI) {}
};



#endif //IMGUIREQUEST_H
