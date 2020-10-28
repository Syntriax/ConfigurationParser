# Configuration Parser
Very simple, Header-only, Key-Value pair Parser and Generator.

To change the default assigner "=" there's a define named CONFIG_ASSIGNER in ConfigReader.h (33).
```C++
#define CONFIG_ASSIGNER "="
```

#### Example
```C++
#include <iostream>
#include "ConfigurationParser.h"

int main()
{
    Syn::ConfigurationParser parser = Syn::ConfigurationParser("config.cfg");
    
    parser["NewKey"] = "NewValue";
    std::cout << "parser[newKey] -> " << parser["NewKey"]  << std::endl;
    
    // Save to the current file "config.cfg"
    parser.SaveFile();

    // Open another file
    parser.ParseFile("anotherConfig.conf");

    for(auto [key, value] : parser)
        std::cout << key << " -> " << value << std::endl;

    // Write to a specific file and change the current file to that file
    parser.WriteFile("newConfig.ini");
    
    return 0;
}
```
