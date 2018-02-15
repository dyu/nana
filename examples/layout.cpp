#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/label.hpp>

static const char LAYOUT[] =
"<main margin=2 grid=[2,1] gap=2>"
;

namespace ui {
    nana::label& label(const std::string& text, nana::form& fm)
    {
        auto l = new nana::label(fm);
        l->caption(text);
        return *l;
    }
}

int main(int argc, char* argv[])
{
    nana::form fm;
    fm.caption("Layout example");
    fm.bgcolor(nana::color_rgb(0xFFFFFF));
    
    nana::place place(fm);
    place.div(LAYOUT);
    
    nana::label left(fm);
    left.caption("left");
    
    place["main"] << left;
    //place["main"] << ui::label("left", fm);
    
    nana::label right(fm);
    right.caption("right");
    
    place["main"] << right;
    
    place.collocate();
    fm.show();
    nana::exec();
    
    return 0;
}
