#include <stdio.h>

#include <caml/mlvalues.h>
#include <caml/bigarray.h>
#include <caml/memory.h>
#include <glad/glad.h>

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>

extern "C" {

    CAMLprim value
    caml_print_hello(value unit)
    {
        printf("Hello \n");
        return Val_unit;
    }

    CAMLprim value
    caml_glfwInit(value unit)
    {
        int ret = glfwInit();
        return Val_bool(ret);
    }

    CAMLprim value
    caml_glfwCreateWindow(value iWidth, value iHeight, value sTitle)
    {
      GLFWwindow* wd;           /* window desciptor/handle */
      int w = Int_val(iWidth);
      int h = Int_val(iHeight);
      char *s = String_val(sTitle);

      wd = glfwCreateWindow(w, h, s,
                            NULL, NULL);
      return (value) wd;
    }

    CAMLprim value
    caml_glfwMakeContextCurrent(value window)
    {
        GLFWwindow* wd = (GLFWwindow *)window;
        glfwMakeContextCurrent(wd);
        gladLoadGLES2Loader((GLADloadproc) glfwGetProcAddress);
        return Val_unit;
    }

    CAMLprim value
    caml_printFrameBufferSize(value window)
    {

        GLFWwindow *wd = (GLFWwindow*)window;
        int fbwidth;
        int fbheight;
        glfwGetFramebufferSize(wd, &fbwidth, &fbheight);

        printf("size2: %d %d\n", fbwidth, fbheight);
        return Val_unit;
    }

    CAMLprim value
    caml_glfwWindowShouldClose(value window)
    {
        GLFWwindow *wd = (GLFWwindow*)window;
        int val = glfwWindowShouldClose(wd);
        return Val_bool(val);
    }

    CAMLprim value
    caml_glfwPollEvents(value unit) 
    {
        glfwPollEvents();
        return Val_unit;
    }

    CAMLprim value
    caml_glClearColor(value vr, value vg, value vb, value va) {
        float r = Double_val(vr);
        float g = Double_val(vg);
        float b = Double_val(vb);
        float a = Double_val(va);
        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT);
        return Val_unit;
    }

    CAMLprim value
    caml_glClearDepth(value vd) {
        float d = Double_val(vd);
        glClearDepthf(d);
        glClear(GL_DEPTH_BUFFER_BIT);
        return Val_unit;
    }

    CAMLprim value
    caml_glEnable(value vEnableOptions) {
        glEnable(GL_DEPTH_TEST);
        return Val_unit;
    }

    CAMLprim value
    caml_glDepthFunc(value vDepthFunc) {
        glDepthFunc(GL_LEQUAL);
        return Val_unit;
    }

    CAMLprim value
    caml_glCreateShader(value v) {
        int shaderType;
        switch (Int_val(v)) {
            case 0:
                printf("vertex shader");
                shaderType = GL_VERTEX_SHADER;
                break;
            default:
            case 1:
                printf("fragment shader");
                shaderType = GL_FRAGMENT_SHADER;
                break;
        }

        return (value) glCreateShader(shaderType);
    }

    CAMLprim value
    caml_glShaderSource(value vShader, value vSource) {
        GLuint shader = (GLuint)vShader;
        char *s;
        s = String_val(vSource);
        glShaderSource(shader, 1, &s, NULL);
        return Val_unit;
    }

    CAMLprim value
    caml_glCompileShader(value vShader) {
        GLuint shader = (GLuint)vShader;
        glCompileShader(shader);
        return Val_unit;
    }

    CAMLprim value
    caml_glDeleteShader(value vShader) {
        GLuint shader = (GLuint)vShader;
        glDeleteShader(shader);
    }

    CAMLprim value
    caml_glGetShaderIsCompiled(value vShader) {
        GLuint shader = (GLuint)vShader;

        GLint result;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

        if (result == GL_TRUE) {
            printf("Compiled! %d\n", shader);
            return Val_true;
        } else {
            printf("Not compiled! %d\n", shader);
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            printf("Output: %s\n", infoLog);
            return Val_false;
        }
    }

    CAMLprim value
    caml_glCreateProgram(value unit) {
        unsigned int shaderProgram;
        shaderProgram = glCreateProgram();
        return (value)shaderProgram;
    }

    CAMLprim value
    caml_glAttachShader(value vProgram, value vShader) {
        unsigned int shaderProgram = (unsigned int)vProgram;
        unsigned int shader = (unsigned int)vShader;
        glAttachShader(shaderProgram, shader);
        return Val_unit;
    }

    CAMLprim value
    caml_glLinkProgram(value vProgram) {
        unsigned int shaderProgram = (unsigned int)vProgram;
        glLinkProgram(shaderProgram);

        int success;
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

        char infoLog[512];
        if (!success) {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            printf("link failed: %s\n", infoLog);
        } else {
            printf("link success!");

            int pos = glGetAttribLocation(shaderProgram, "aVertexPosition");
            printf("- pos: %d\n", pos);
        }
        return Val_unit;
    }

    CAMLprim value
    caml_glUseProgram(value vProgram) {
        unsigned int program = (unsigned int)vProgram;
        glUseProgram(program);
        return Val_unit;
    }

    CAMLprim value
    caml_glCreateBuffer(value unit) {
        unsigned int VBO;
        glGenBuffers(1, &VBO);
        printf("glCreateBuffer: %d \n", VBO);
        return (value)VBO;
    }

    CAMLprim value
    caml_glBindBuffer(value vBufferType, value vBuffer) {
        unsigned int VBO = (unsigned int)vBuffer;
        /* printf("bind buffer: %d\n", VBO); */

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        /* printf("glBindBuffer: %d\n", VBO); */
        return Val_unit;
    }

    CAMLprim value
    caml_glBufferData(value vBufferType, value vSize, value vData, value drawType) {
        /* printf("glBufferData - show floats TODO\n"); */
        int val = Caml_ba_array_val(vData)->dim[0];
        /* printf("-size: %d\n -num dimensions: %d\n", val, Caml_ba_array_val(vData)->num_dims); */
        float* elements = (float*)Caml_ba_data_val(vData);
        /* for (int i = 0; i < val; i++) { */
        /*     printf("- element %d : %f", i, *(elements++)); */
        /* } */
        glBufferData(GL_ARRAY_BUFFER, val * sizeof(float), elements, GL_STATIC_DRAW);
        return Val_unit;
    }

    CAMLprim value
    caml_glDrawArrays(value vDrawMode, value vFirst, value vCount) {
        unsigned int first = Int_val(vFirst);
        unsigned int count = Int_val(vCount);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        return Val_unit;
    }

    CAMLprim value
    caml_glEnableVertexAttribArray(value unit) {
        glEnableVertexAttribArray(0);
        return Val_unit;
    }

    CAMLprim value
    caml_glVertexAttribPointer(value unit) {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        return Val_unit;
    }

    CAMLprim value
    caml_glUnbindBuffer(value unit) {
        printf("TODO: glUnbindBuffer\n");
        return Val_unit;
    }

    CAMLprim value
    caml_glfwSwapBuffers(value window)
    {
        GLFWwindow *wd = (GLFWwindow*)window;
        glfwSwapBuffers(wd);
        return Val_unit;
    }

    CAMLprim value
    caml_glfwTerminate(value unit)
    {
        glfwTerminate();
        return Val_unit;
    }
}
