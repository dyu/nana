#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/label.hpp>

int main()
{
    nana::form fm;
    fm.caption("Layout example");
    fm.bgcolor(nana::color_rgb(0xFFFFFF));
    
    nana::place place(fm);
    
    fm.show();
    nana::exec();
}
