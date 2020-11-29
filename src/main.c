#include "cube3.h"

// The global window object
GLFWwindow *window;

// Vertices for a square
float vertices[] = {
    -0.05f, -0.05f, 0.0f, // Bottom-left
    -0.05f, 0.05f,  0.0f, // Top-left
    0.05f,  0.05f,  0.0f, // Top-right

    0.05f,  0.05f,  0.0f, // Top-right
    0.05f,  -0.05f, 0.0f, // Bottom-right
    -0.05f, -0.05f, 0.0f, // Bottom-left
};

// Set the cube orientation at identity:
//
//  |1 0 0 0|
//  |0 1 0 0|
//  |0 0 1 0|
//  |0 0 0 1|
//

Model cube = { .orientation = MATRIX_IDENTITY };

static void playerMovement(float speed)
{
    // Test for constant movement
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        cube.orientation.tX += speed * HORIZONTAL_DIV;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        cube.orientation.tX -= speed * HORIZONTAL_DIV;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        cube.orientation.tY += speed * VERTICAL_DIV;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        cube.orientation.tY -= speed * VERTICAL_DIV;
    }

    // Prevent infinite translations
    if (cube.orientation.tX > 1.0f)
    {
        cube.orientation.tX = -1.0f;
    }
    else if (cube.orientation.tX < -1.0f)
    {
        cube.orientation.tX = 1.0f;
    }

    if (cube.orientation.tY > 1.0f)
    {
        cube.orientation.tY = -1.0f;
    }
    else if (cube.orientation.tY < -1.0f)
    {
        cube.orientation.tY = 1.0f;
    }
}

static void gameLoop()
{
    unsigned int shaderProgram;
    unsigned int VBO, VAO;
    unsigned int orientationMatrix;
    double       expectedFrameTime;

    // Import shaders
    shaderProgram = linkShaders(importFragmentShader("shaders/fragment.glsl"),
                                importVertexShader("shaders/vertex.glsl"));

    // Initialize VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s)
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound
    // vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens.
    glBindVertexArray(0);

    // Use the shader program
    glUseProgram(shaderProgram);

    // Set the uniform
    orientationMatrix = glGetUniformLocation(shaderProgram, "Transform");

    // Set the expected frame time
    expectedFrameTime = glfwGetTime() + DESIRED_FRAME_TIME;

    while (1)
    {
        if (glfwWindowShouldClose(window))
        {
            // Exit the game loop
            break;
        }

        // Get the current time in microseconds
        // currentFrameTime = glfwGetTime();

        // Compute the difference of between two frame times
        // deltaFrameTime = currentFrameTime - previousFrameTime;

        // The previous frame time becomes the current frame time
        // previousFrameTime = currentFrameTime;

        while (glfwGetTime() < expectedFrameTime)
        {
            // Do nothing
        }

        // Reset the expected frame time
        expectedFrameTime = glfwGetTime() + DESIRED_FRAME_TIME;

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Test for constant movement at 25 px /frame
        playerMovement(25.0f);

        // Pass uniform as the transpose matrix with GL_TRUE parameter
        glUniformMatrix4fv(orientationMatrix, 1, GL_TRUE, (float *) &cube.orientation);

        // Draw the red square
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Free memory
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
}

int main()
{
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // Prevent resize

    // Creates the window
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Cube3", NULL, NULL);

    if ((window) == NULL)
    {
        fprintf(stderr, "Failed to open GLFW window.\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // The callback routine is hellish slow
    // glfwSetKeyCallback(window, &rsKeyboardCallback);

    gameLoop();

    glfwTerminate();
    return 0;
}
