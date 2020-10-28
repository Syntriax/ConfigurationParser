/*
    MIT License

    Copyright (c) 2020 Syntriax

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#pragma once

#include <iostream>
#include <fstream>
#include <map>

namespace Syn
{   
    #define CONFIG_ASSIGNER "="

    class ConfigurationParser
    {
        private:
            const char skips[5] = { '#', '[', ';', '\n', '\0' };
            std::string filePath;
            std::map<std::string, std::string> contents;
            void ProcessLine(const std::string &);

        public:
            ConfigurationParser();
            ConfigurationParser(const std::string &);
            ~ConfigurationParser();
            
            void Clear();

            bool ParseFile(const std::string &);
            bool WriteFile(const std::string &);
            bool SaveFile();

            std::map<std::string, std::string>::iterator begin();
            std::map<std::string, std::string>::iterator end();
            
            std::string &operator[](const char *);
    };
    
    ConfigurationParser::ConfigurationParser() { Clear(); }
    ConfigurationParser::ConfigurationParser(const std::string &filePath) { ParseFile(filePath); }
    ConfigurationParser::~ConfigurationParser() { }
    
    void ConfigurationParser::ProcessLine(const std::string &line) 
    {
        for(char i : skips)
            if(line[0] == i)
                return;
        
        size_t assignerPosition = line.find(CONFIG_ASSIGNER);
        size_t assignerLenght = std::string(CONFIG_ASSIGNER).length();

        if(assignerPosition == std::string::npos)
        {
            std::cerr << "SynConfigurationParser: Can't find the assigner(" << CONFIG_ASSIGNER << ")" << 
                        " in line: \"" << line << 
                        "\" at \"" << filePath << "\"" << std::endl;
            return;
        }
        
        std::string key = line.substr(0, assignerPosition);
        std::string value = line.substr(assignerPosition + assignerLenght, line.size() - assignerPosition - assignerLenght);
        try
        {
            contents[key.c_str()] = value;
        }
        catch(const std::exception& e)
        {
            throw std::runtime_error(std::string("SynConfigurationParser: Error while reading file at ") + filePath + "\". Key: " + key + ". Value: " + value);
        }
    }

    void ConfigurationParser::Clear() 
    {
        filePath = "";
        contents.clear();
    }

    bool ConfigurationParser::ParseFile(const std::string &filePath)
    {
        Clear();
        std::fstream stream = std::fstream(filePath);

        if(!stream.good())
        {
            std::cerr << "SynConfigurationParser: Can't read the file located at \"" << filePath << "\"" << std::endl;;
            return false;
        }

        this -> filePath = filePath;
        
        std::string line;
        while(std::getline(stream, line))
            ProcessLine(line);

        stream.close();
        return true;
    }

    bool ConfigurationParser::WriteFile(const std::string &filePath)
    {
        std::fstream stream = std::fstream(filePath, std::ios::out | std::ios::trunc);
         
        if(!stream.good())
        {
            std::cerr << "SynConfigurationParser: Can't write to the file located at \"" << filePath << "\"" << std::endl;;
            return false;
        }

        for (auto const& [key, value] : contents)
            stream << key << CONFIG_ASSIGNER << value << std::endl; 

        this -> filePath = filePath;
        stream.close();
        return true;
    }

    bool ConfigurationParser::SaveFile() { return WriteFile(filePath); }
    
    std::map<std::string, std::string>::iterator ConfigurationParser::begin() { return contents.begin(); }
    std::map<std::string, std::string>::iterator ConfigurationParser::end()   { return contents.end();   }

    std::string &ConfigurationParser::operator[](const char *key) { return contents[key]; }
} // namespace Syn
