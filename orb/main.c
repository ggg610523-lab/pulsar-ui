#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
 * IMPORTANT:
 * GLEW must be included before SDL_opengl.h.
 */
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#define WIDTH  900
#define HEIGHT 700

static const char *vertex_shader_src =
"#version 330 core\n"

"layout(location = 0) in vec2 aPos;\n"

"out vec2 uv;\n"

"void main()\n"
"{\n"
"    uv = aPos * 0.5 + 0.5;\n"
"    gl_Position = vec4(aPos, 0.0, 1.0);\n"
"}\n";


static const char *fragment_shader_src =
"#version 330 core\n"

"out vec4 FragColor;\n"

"in vec2 uv;\n"

"uniform float uTime;\n"
"uniform float uAudio;\n"
"uniform vec2 uResolution;\n"

"#define PI 3.14159265359\n"


/*
 * ---------------------------------------------------------
 * HASH
 * ---------------------------------------------------------
 */

"float hash21(vec2 p)\n"
"{\n"
"    p = fract(p * vec2(123.34, 456.21));\n"
"    p += dot(p, p + 45.32);\n"
"    return fract(p.x * p.y);\n"
"}\n"


/*
 * ---------------------------------------------------------
 * VALUE NOISE
 * ---------------------------------------------------------
 */

"float noise(vec2 p)\n"
"{\n"
"    vec2 i = floor(p);\n"
"    vec2 f = fract(p);\n"

"    f = f * f * (3.0 - 2.0 * f);\n"

"    float a = hash21(i);\n"
"    float b = hash21(i + vec2(1.0, 0.0));\n"
"    float c = hash21(i + vec2(0.0, 1.0));\n"
"    float d = hash21(i + vec2(1.0, 1.0));\n"

"    return mix(\n"
"        mix(a, b, f.x),\n"
"        mix(c, d, f.x),\n"
"        f.y\n"
"    );\n"
"}\n"


/*
 * ---------------------------------------------------------
 * FRACTAL BROWNIAN MOTION
 * ---------------------------------------------------------
 */

"float fbm(vec2 p)\n"
"{\n"
"    float value = 0.0;\n"
"    float amplitude = 0.5;\n"

"    for(int i = 0; i < 5; i++)\n"
"    {\n"
"        value += noise(p) * amplitude;\n"
"        p *= 2.0;\n"
"        amplitude *= 0.5;\n"
"    }\n"

"    return value;\n"
"}\n"


/*
 * ---------------------------------------------------------
 * SIRI-STYLE COLOR PALETTE
 *
 * magenta -> purple -> blue -> cyan -> pink
 * ---------------------------------------------------------
 */

"vec3 palette(float x)\n"
"{\n"
"    vec3 magenta = vec3(1.00, 0.02, 0.42);\n"
"    vec3 purple  = vec3(0.55, 0.05, 1.00);\n"
"    vec3 blue    = vec3(0.04, 0.20, 1.00);\n"
"    vec3 cyan    = vec3(0.00, 0.85, 1.00);\n"
"    vec3 pink    = vec3(1.00, 0.30, 0.75);\n"

"    x = fract(x);\n"

"    if(x < 0.20)\n"
"        return mix(\n"
"            magenta,\n"
"            purple,\n"
"            x / 0.20\n"
"        );\n"

"    if(x < 0.45)\n"
"        return mix(\n"
"            purple,\n"
"            blue,\n"
"            (x - 0.20) / 0.25\n"
"        );\n"

"    if(x < 0.70)\n"
"        return mix(\n"
"            blue,\n"
"            cyan,\n"
"            (x - 0.45) / 0.25\n"
"        );\n"

"    return mix(\n"
"        cyan,\n"
"        pink,\n"
"        (x - 0.70) / 0.30\n"
"    );\n"
"}\n"


/*
 * ---------------------------------------------------------
 * MAIN FRAGMENT SHADER
 * ---------------------------------------------------------
 */

"void main()\n"
"{\n"

"    //-----------------------------------------------------\n"
"    // Center coordinates\n"
"    //-----------------------------------------------------\n"

"    vec2 p = uv - 0.5;\n"

"    float aspect = uResolution.x / uResolution.y;\n"

"    p.x *= aspect;\n"


"    //-----------------------------------------------------\n"
"    // Orb dimensions\n"
"    //-----------------------------------------------------\n"

"    float audio = uAudio;\n"

"    float radius = 0.285;\n"

"    radius += audio * 0.018;\n"

"    float r = length(p);\n"


"    //-----------------------------------------------------\n"
"    // Sphere mask\n"
"    //-----------------------------------------------------\n"

"    float inside = 1.0 - smoothstep(\n"
"        radius,\n"
"        radius + 0.012,\n"
"        r\n"
"    );\n"


"    //-----------------------------------------------------\n"
"    // Time\n"
"    //-----------------------------------------------------\n"

"    float t = uTime;\n"


"    //-----------------------------------------------------\n"
"    // Normalized orb coordinates\n"
"    //-----------------------------------------------------\n"

"    vec2 q = p / radius;\n"

"    float qlen = length(q);\n"

"    float sphereZ = sqrt(\n"
"        max(\n"
"            0.0,\n"
"            1.0 - qlen * qlen\n"
"        )\n"
"    );\n"


"    //-----------------------------------------------------\n"
"    // Liquid distortion\n"
"    //-----------------------------------------------------\n"

"    vec2 flow = q;\n"

"    flow.x += sin(\n"
"        q.y * 4.0 + t * 0.75\n"
"    ) * 0.10;\n"

"    flow.y += cos(\n"
"        q.x * 5.0 - t * 0.62\n"
"    ) * 0.08;\n"


"    float n1 = fbm(\n"
"        flow * 2.7 +\n"
"        vec2(t * 0.08, -t * 0.05)\n"
"    );\n"


"    float n2 = fbm(\n"
"        flow * 5.0 -\n"
"        vec2(t * 0.12, t * 0.09)\n"
"    );\n"


"    //-----------------------------------------------------\n"
"    // LARGE FLOWING RIBBON\n"
"    //-----------------------------------------------------\n"

"    float wave1 = sin(\n"
"        q.x * 4.0 +\n"
"        q.y * 2.0 +\n"
"        t * 1.25 +\n"
"        n1 * 2.2\n"
"    );\n"


"    float ribbon1 = exp(\n"
"        -abs(wave1) * 2.8\n"
"    );\n"


"    //-----------------------------------------------------\n"
"    // SECONDARY RIBBON\n"
"    //-----------------------------------------------------\n"

"    float wave2 = sin(\n"
"        q.x * 7.0 -\n"
"        q.y * 3.0 -\n"
"        t * 1.6 +\n"
"        n2 * 3.0\n"
"    );\n"


"    float ribbon2 = exp(\n"
"        -abs(wave2) * 4.0\n"
"    );\n"


"    //-----------------------------------------------------\n"
"    // THIRD FINE RIBBON\n"
"    //-----------------------------------------------------\n"

"    float wave3 = sin(\n"
"        q.x * 12.0 +\n"
"        q.y * 5.0 +\n"
"        t * 2.2 +\n"
"        n2 * 5.0\n"
"    );\n"


"    float ribbon3 = exp(\n"
"        -abs(wave3) * 7.0\n"
"    );\n"


"    //-----------------------------------------------------\n"
"    // COLOR POSITION\n"
"    //-----------------------------------------------------\n"

"    float colorPos =\n"
"        q.x * 0.38 +\n"
"        q.y * 0.25 +\n"
"        n1 * 0.55 +\n"
"        t * 0.035;\n"


"    vec3 color1 = palette(colorPos);\n"

"    vec3 color2 = palette(colorPos + 0.37);\n"

"    vec3 color3 = palette(colorPos + 0.67);\n"


"    //-----------------------------------------------------\n"
"    // Combine ribbons\n"
"    //-----------------------------------------------------\n"

"    vec3 color = color1;\n"

"    color = mix(\n"
"        color,\n"
"        color2,\n"
"        ribbon2 * 0.50\n"
"    );\n"

"    color = mix(\n"
"        color,\n"
"        color3,\n"
"        ribbon3 * 0.20\n"
"    );\n"


"    //-----------------------------------------------------\n"
"    // Audio response\n"
"    //-----------------------------------------------------\n"

"    float intensity =\n"
"        ribbon1 * 0.95 +\n"
"        ribbon2 * 0.45 +\n"
"        ribbon3 * 0.18;\n"

"    intensity *=\n"
"        0.75 + audio * 0.90;\n"


"    //-----------------------------------------------------\n"
"    // Spherical lighting\n"
"    //-----------------------------------------------------\n"

"    vec3 normal = normalize(\n"
"        vec3(q, sphereZ)\n"
"    );\n"


"    vec3 lightDir = normalize(\n"
"        vec3(-0.35, -0.45, 1.0)\n"
"    );\n"


"    float diffuse = max(\n"
"        dot(normal, lightDir),\n"
"        0.0\n"
"    );\n"


"    color *=\n"
"        0.72 + diffuse * 0.55;\n"


"    //-----------------------------------------------------\n"
"    // Fresnel glass edge\n"
"    //-----------------------------------------------------\n"

"    float fresnel = pow(\n"
"        1.0 - max(normal.z, 0.0),\n"
"        3.5\n"
"    );\n"


"    color +=\n"
"        vec3(0.25, 0.65, 1.0) *\n"
"        fresnel * 0.75;\n"


"    //-----------------------------------------------------\n"
"    // Bright center\n"
"    //-----------------------------------------------------\n"

"    float centerGlow = exp(\n"
"        -r * r * 35.0\n"
"    );\n"


"    color +=\n"
"        vec3(1.0, 0.55, 0.85) *\n"
"        centerGlow * 1.8;\n"


"    //-----------------------------------------------------\n"
"    // White-hot core\n"
"    //-----------------------------------------------------\n"

"    float whiteCore = exp(\n"
"        -r * r * 80.0\n"
"    );\n"


"    color +=\n"
"        vec3(1.0, 0.90, 0.98) *\n"
"        whiteCore * 1.5;\n"


"    //-----------------------------------------------------\n"
"    // Inner blue glow\n"
"    //-----------------------------------------------------\n"

"    float innerGlow = exp(\n"
"        -r * r * 12.0\n"
"    );\n"


"    color +=\n"
"        vec3(0.45, 0.75, 1.0) *\n"
"        innerGlow * 0.65;\n"


"    //-----------------------------------------------------\n"
"    // Apply ribbon intensity\n"
"    //-----------------------------------------------------\n"

"    color *= intensity;\n"


"    //-----------------------------------------------------\n"
"    // OUTER BLOOM\n"
"    //-----------------------------------------------------\n"

"    float glow = exp(\n"
"        -r * r * 10.0\n"
"    );\n"


"    vec3 glowColor =\n"
"        vec3(0.18, 0.28, 1.0);\n"


"    glowColor +=\n"
"        vec3(0.65, 0.05, 0.55) * 0.45;\n"


"    vec3 finalColor =\n"
"        color * inside;\n"


"    finalColor +=\n"
"        glowColor * glow * 0.35;\n"


"    //-----------------------------------------------------\n"
"    // Audio bloom\n"
"    //-----------------------------------------------------\n"

"    float audioGlow =\n"
"        exp(-r * r * 7.0) * audio;\n"


"    finalColor +=\n"
"        vec3(0.15, 0.45, 1.0) *\n"
"        audioGlow * 0.45;\n"


"    //-----------------------------------------------------\n"
"    // Soft outer rim\n"
"    //-----------------------------------------------------\n"

"    float edge = smoothstep(\n"
"        radius - 0.035,\n"
"        radius,\n"
"        r\n"
"    );\n"


"    finalColor +=\n"
"        vec3(0.25, 0.55, 1.0) *\n"
"        edge * 0.55;\n"


"    //-----------------------------------------------------\n"
"    // Prevent negative colors\n"
"    //-----------------------------------------------------\n"

"    finalColor = max(\n"
"        finalColor,\n"
"        vec3(0.0)\n"
"    );\n"


"    FragColor = vec4(\n"
"        finalColor,\n"
"        1.0\n"
"    );\n"
"}\n";


/*
 * =========================================================
 * SHADER COMPILATION
 * =========================================================
 */

static GLuint compile_shader(
    GLenum type,
    const char *source
)
{
    GLuint shader =
        glCreateShader(type);

    glShaderSource(
        shader,
        1,
        &source,
        NULL
    );

    glCompileShader(shader);

    GLint success = 0;

    glGetShaderiv(
        shader,
        GL_COMPILE_STATUS,
        &success
    );

    if (!success)
    {
        char log[4096];

        glGetShaderInfoLog(
            shader,
            sizeof(log),
            NULL,
            log
        );

        fprintf(
            stderr,
            "\nShader compilation failed:\n%s\n",
            log
        );

        glDeleteShader(shader);

        exit(EXIT_FAILURE);
    }

    return shader;
}


/*
 * =========================================================
 * CREATE OPENGL PROGRAM
 * =========================================================
 */

static GLuint create_program(void)
{
    GLuint vertexShader =
        compile_shader(
            GL_VERTEX_SHADER,
            vertex_shader_src
        );

    GLuint fragmentShader =
        compile_shader(
            GL_FRAGMENT_SHADER,
            fragment_shader_src
        );

    GLuint program =
        glCreateProgram();

    glAttachShader(
        program,
        vertexShader
    );

    glAttachShader(
        program,
        fragmentShader
    );

    glLinkProgram(program);

    GLint success = 0;

    glGetProgramiv(
        program,
        GL_LINK_STATUS,
        &success
    );

    if (!success)
    {
        char log[4096];

        glGetProgramInfoLog(
            program,
            sizeof(log),
            NULL,
            log
        );

        fprintf(
            stderr,
            "\nOpenGL program linking failed:\n%s\n",
            log
        );

        exit(EXIT_FAILURE);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}


/*
 * =========================================================
 * MAIN
 * =========================================================
 */

int main(void)
{
    /*
     * -----------------------------------------------------
     * SDL initialization
     * -----------------------------------------------------
     */

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(
            stderr,
            "SDL_Init failed: %s\n",
            SDL_GetError()
        );

        return EXIT_FAILURE;
    }


    /*
     * -----------------------------------------------------
     * Request OpenGL 3.3 Core
     * -----------------------------------------------------
     */

    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_MAJOR_VERSION,
        3
    );

    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_MINOR_VERSION,
        3
    );

    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_PROFILE_MASK,
        SDL_GL_CONTEXT_PROFILE_CORE
    );

    SDL_GL_SetAttribute(
        SDL_GL_DOUBLEBUFFER,
        1
    );


    /*
     * -----------------------------------------------------
     * Create window
     * -----------------------------------------------------
     */

    SDL_Window *window =
        SDL_CreateWindow(
            "Siri Orb",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            WIDTH,
            HEIGHT,
            SDL_WINDOW_OPENGL |
            SDL_WINDOW_RESIZABLE
        );

    if (!window)
    {
        fprintf(
            stderr,
            "SDL_CreateWindow failed: %s\n",
            SDL_GetError()
        );

        SDL_Quit();

        return EXIT_FAILURE;
    }


    /*
     * -----------------------------------------------------
     * Create OpenGL context
     * -----------------------------------------------------
     */

    SDL_GLContext context =
        SDL_GL_CreateContext(window);

    if (!context)
    {
        fprintf(
            stderr,
            "SDL_GL_CreateContext failed: %s\n",
            SDL_GetError()
        );

        SDL_DestroyWindow(window);
        SDL_Quit();

        return EXIT_FAILURE;
    }


    /*
     * -----------------------------------------------------
     * Initialize GLEW
     * -----------------------------------------------------
     */

    glewExperimental = GL_TRUE;

    GLenum glewError =
        glewInit();

    if (glewError != GLEW_OK)
    {
        fprintf(
            stderr,
            "GLEW initialization failed: %s\n",
            glewGetErrorString(glewError)
        );

        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);
        SDL_Quit();

        return EXIT_FAILURE;
    }


    /*
     * GLEW can generate a harmless GL_INVALID_ENUM
     * during initialization on some systems.
     */

    glGetError();


    /*
     * -----------------------------------------------------
     * Print GPU information
     * -----------------------------------------------------
     */

    printf(
        "OpenGL:  %s\n",
        glGetString(GL_VERSION)
    );

    printf(
        "Renderer: %s\n",
        glGetString(GL_RENDERER)
    );

    printf(
        "GLSL:    %s\n",
        glGetString(GL_SHADING_LANGUAGE_VERSION)
    );


    /*
     * -----------------------------------------------------
     * VSync
     * -----------------------------------------------------
     */

    SDL_GL_SetSwapInterval(1);


    /*
     * -----------------------------------------------------
     * Create shader program
     * -----------------------------------------------------
     */

    GLuint program =
        create_program();


    /*
     * -----------------------------------------------------
     * Fullscreen triangle
     *
     * A triangle is preferable to a quad because it
     * covers the entire screen without diagonal seams.
     * -----------------------------------------------------
     */

    float vertices[] =
    {
        -1.0f, -1.0f,
         3.0f, -1.0f,
        -1.0f,  3.0f
    };


    GLuint vao = 0;
    GLuint vbo = 0;


    glGenVertexArrays(
        1,
        &vao
    );

    glGenBuffers(
        1,
        &vbo
    );


    glBindVertexArray(vao);


    glBindBuffer(
        GL_ARRAY_BUFFER,
        vbo
    );


    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW
    );


    glVertexAttribPointer(
        0,
        2,
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(float),
        (void *)0
    );


    glEnableVertexAttribArray(0);


    glBindBuffer(
        GL_ARRAY_BUFFER,
        0
    );

    glBindVertexArray(0);


    /*
     * -----------------------------------------------------
     * Uniform locations
     * -----------------------------------------------------
     */

    GLint timeLocation =
        glGetUniformLocation(
            program,
            "uTime"
        );


    GLint audioLocation =
        glGetUniformLocation(
            program,
            "uAudio"
        );


    GLint resolutionLocation =
        glGetUniformLocation(
            program,
            "uResolution"
        );


    /*
     * -----------------------------------------------------
     * Main loop
     * -----------------------------------------------------
     */

    int running = 1;

    Uint64 start =
        SDL_GetPerformanceCounter();


    while (running)
    {
        /*
         * -------------------------------------------------
         * Events
         * -------------------------------------------------
         */

        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = 0;
            }

            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    running = 0;
                }
            }
        }


        /*
         * -------------------------------------------------
         * Time
         * -------------------------------------------------
         */

        Uint64 now =
            SDL_GetPerformanceCounter();


        float time =
            (float)(now - start) /
            (float)SDL_GetPerformanceFrequency();


        /*
         * -------------------------------------------------
         * Fake microphone level
         *
         * This is currently simulated.
         *
         * Later this gets replaced by actual microphone
         * RMS/FFT information.
         * -------------------------------------------------
         */

        float audio =
            0.50f
            + 0.25f * sinf(time * 2.1f)
            + 0.15f * sinf(time * 5.7f)
            + 0.08f * sinf(time * 11.0f);


        audio =
            fmaxf(
                0.0f,
                fminf(
                    1.0f,
                    audio
                )
            );


        /*
         * -------------------------------------------------
         * Window dimensions
         * -------------------------------------------------
         */

        int width;
        int height;


        SDL_GetWindowSize(
            window,
            &width,
            &height
        );


        /*
         * -------------------------------------------------
         * Viewport
         * -------------------------------------------------
         */

        glViewport(
            0,
            0,
            width,
            height
        );


        /*
         * -------------------------------------------------
         * Black background
         * -------------------------------------------------
         */

        glClearColor(
            0.0f,
            0.0f,
            0.008f,
            1.0f
        );


        glClear(
            GL_COLOR_BUFFER_BIT
        );


        /*
         * -------------------------------------------------
         * Shader
         * -------------------------------------------------
         */

        glUseProgram(program);


        glUniform1f(
            timeLocation,
            time
        );


        glUniform1f(
            audioLocation,
            audio
        );


        glUniform2f(
            resolutionLocation,
            (float)width,
            (float)height
        );


        /*
         * -------------------------------------------------
         * Draw fullscreen triangle
         * -------------------------------------------------
         */

        glBindVertexArray(vao);

        glDrawArrays(
            GL_TRIANGLES,
            0,
            3
        );

        glBindVertexArray(0);


        /*
         * -------------------------------------------------
         * Present
         * -------------------------------------------------
         */

        SDL_GL_SwapWindow(window);
    }


    /*
     * -----------------------------------------------------
     * Cleanup
     * -----------------------------------------------------
     */

    glDeleteBuffers(
        1,
        &vbo
    );


    glDeleteVertexArrays(
        1,
        &vao
    );


    glDeleteProgram(
        program
    );


    SDL_GL_DeleteContext(
        context
    );


    SDL_DestroyWindow(
        window
    );


    SDL_Quit();


    return EXIT_SUCCESS;
}
