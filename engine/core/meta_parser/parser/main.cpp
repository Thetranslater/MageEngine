#include "common/precompiled.h"
#include "parser/parser.h"

int parse(std::string project_file_name,
          std::string source_include_file_name,
          std::string include_path,
          std::string sys_include,
          std::string module_name,
          std::string show_errors);

int main(int argc, char* argv[])
{
    auto start_time = std::chrono::system_clock::now();
    int  result     = 0;

    /*if (argv[1] != nullptr && argv[2] != nullptr && argv[3] != nullptr && argv[4] != nullptr && argv[5] != nullptr &&
        argv[6] != nullptr)*/
    {   
        MetaParser::prepare();

        result = parse("E:/Mage/engine/precompile.json",
                       "E:/Mage/engine/parser_header.h", 
                       "E:/Mage/engine/core",
                       "*",
                       "Mage",
                       "1");

        auto duration_time = std::chrono::system_clock::now() - start_time;
        std::cout << "Completed in " << std::chrono::duration_cast<std::chrono::milliseconds>(duration_time).count()
                  << "ms" << std::endl;
        return result;
    }
    /*else
    {
        std::cerr << "Arguments parse error!" << std::endl
                  << "Please call the tool like this:" << std::endl
                  << "meta_parser  project_file_name  include_file_name_to_generate  project_base_directory "
                     "sys_include_directory module_name showErrors(0 or 1)"
                  << std::endl
                  << std::endl;
        return -1;
    }*/

    return 0;
}

int parse(std::string project_input_file_name,  /*    file: ../engine/bin/precompile.json    */
          std::string source_include_file_name, /*    file: ../build/parser_header.h         */
          std::string include_path,             /*    file: ../engine/source                 */
          std::string sys_include,              /*    file: *                                */
          std::string module_name,              /*    Pilot                                  */
          std::string show_errors)
{
    std::cout << std::endl;
    std::cout << "Parsing meta data for target \"" << module_name << "\"" << std::endl;
    std::fstream input_file;

    bool is_show_errors = "0" != show_errors;

    MetaParser parser(
        project_input_file_name, source_include_file_name, include_path, sys_include, module_name, is_show_errors);

    std::cout << "Parsing in " << include_path << std::endl;
    int result = parser.parse();
    if (0 != result)
    {
        return result;
    }

    parser.generateFiles();

    return 0;
}
