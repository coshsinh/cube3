#include "cube3.h"

static char const *readShaderFile(char const *pathname)
{
    int    fd;
    size_t size;
    char * buffer;

    // Open the shader file in read-only mode
    if ((fd = open(pathname, O_RDONLY)) == -1)
    {
        perror(pathname);
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Place the cursor at the end of the file to get the size
    size = lseek(fd, 0, SEEK_END);

    if (!(buffer = malloc(sizeof(char) * (size + 1))))
    {
        fprintf(stderr, "Failed to allocate memory: %s\n", strerror(errno));
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Rewind the cursor at the beginning of the file and read the content into the buffer
    lseek(fd, 0, SEEK_SET);
    read(fd, buffer, size);

    // Null-terminate the source code string
    buffer[size] = '\0';

    // Close the shader file descriptor
    close(fd);

    return buffer;
}

static void compileShaderFile(unsigned int shader, char const *pathname)
{
    char const *source;
    int         isCompiled;
    int         maxLength;
    char *      errorLog;

    // Get the source code from the shader file
    source = readShaderFile(pathname);

    // Replaces the source code in a shader object
    glShaderSource(shader, 1, &source, NULL);

    // Compile the shader object
    glCompileShader(shader);

    // Get the compilation status
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

    if (isCompiled == GL_FALSE)
    {
        // Get the size for the error log buffer
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        if (!(errorLog = malloc(sizeof(char) * maxLength)))
        {
            fprintf(stderr, "Failed to allocate memory: %s\n", strerror(errno));
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        // Get the compilation error log
        glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
        fprintf(stderr, "Shader compilation error: %s\n", errorLog);

        // The shader is useless now
        glDeleteShader(shader);

        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Free the source code buffer
    free((void *) source);
}

unsigned int importFragmentShader(char const *pathname)
{
    unsigned int shader;

    // Create the fragment shader object
    shader = glCreateShader(GL_FRAGMENT_SHADER);

    // Compile the shader file
    compileShaderFile(shader, pathname);

    return shader;
}

unsigned int importVertexShader(char const *pathname)
{
    unsigned int shader;

    // Create the fragment shader object
    shader = glCreateShader(GL_VERTEX_SHADER);

    // Compile the shader file
    compileShaderFile(shader, pathname);

    return shader;
}

unsigned int linkShaders(unsigned int fragmentShader, unsigned int vertexShader)
{
    unsigned int program;
    int          isLinked;
    int          maxLength;
    char *       errorLog;

    // Create the program object
    program = glCreateProgram();

    // Attach the fragment and vertex shader to the program object
    glAttachShader(program, fragmentShader);
    glAttachShader(program, vertexShader);

    // Link the fragment and vertex shader
    glLinkProgram(program);

    // Get the linker status
    glGetShaderiv(program, GL_COMPILE_STATUS, &isLinked);

    if (isLinked == GL_FALSE)
    {
        // Get the size for the error log buffer
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        if (!(errorLog = malloc(sizeof(char) * maxLength)))
        {
            fprintf(stderr, "Failed to allocate memory: %s\n", strerror(errno));
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        // Get the compilation error log
        glGetProgramInfoLog(program, maxLength, &maxLength, &errorLog[0]);
        fprintf(stderr, "Shader linker error: %s\n", errorLog);

        // The program is useless now
        glDeleteProgram(program);

        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Do not leak memory
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);

    return program;
}
