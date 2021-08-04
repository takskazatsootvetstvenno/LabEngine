#ifndef SHADER_PROGRAM_HPP
#define SHADER_PROGRAM_HPP
namespace LabEngine {
    class ShaderProgram {
    public:
        ShaderProgram(const char* vertex_shader_src, const char* fragment_shader_src);
        ShaderProgram(ShaderProgram&&);
        ShaderProgram& operator=(ShaderProgram&&);
        ~ShaderProgram();

        ShaderProgram() = delete;
        ShaderProgram(const ShaderProgram&) = delete; //убран копирующий конструктор
        ShaderProgram& operator=(const ShaderProgram&) = delete;

        void bind() const;
        static void unbind();
        bool isCompiled() const { return m_isCompiled; }

    private:
        bool m_isCompiled = false;
        unsigned int m_id = 0;
    };

}  // namespace LabEngine
#endif  // !SHADER_PROGRAM_HPP
