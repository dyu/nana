#include <nana/gui/wvl.hpp>
#include <nana/gui/animation.hpp>

extern "C" {
#include <sys/stat.h>
}

static bool exists(const char *path)
{
    struct stat b;
    return 0 == stat(path, &b);
}

int main()
{
    using namespace nana;
    
    std::string assets_dir;
    if (!exists("gn-out"))
      assets_dir += "../";
    assets_dir += "examples/assets";
   
    //Build frames
    frameset fset;
    fset.push_back(nana::paint::image(assets_dir + "/a_pic0.bmp"));
    fset.push_back(nana::paint::image(assets_dir + "/a_pic1.bmp"));
    fset.push_back(nana::paint::image(assets_dir + "/a_pic2.bmp"));

    //A widget to display animation.
    form fm;
    fm.show();

    animation ani;
    ani.push_back(fset);
    ani.output(fm, nana::point());
    ani.looped(true);
    ani.play();

    exec();
}
