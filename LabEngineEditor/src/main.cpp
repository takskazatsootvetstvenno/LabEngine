#include <iostream>
#include <memory>
#include "Application.hpp"
#include "Log.hpp"
#include "Window.hpp"
class MyApp : public LabEngine::Application {
    virtual void on_update() override {
      //  std::cout << frame<<std::endl;
        frame++;
       // LOG_INFO(flo());
        // std::cout << "Update frame: " << frame++<< std::endl;
    }
    int frame;
    
};
int main() {
    auto myApp = std::make_unique<MyApp>();
    int returnCode = myApp->start(640, 480, "first");
    // system("pause");
    return returnCode;
}
