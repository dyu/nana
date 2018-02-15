#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/label.hpp>

static const char LAYOUT[] =
//"<main_ margin=2 grid=[2,1] gap=2>"
//"<left_>|70%<right_>"
//"<left_><right_ weight=70%>"
"vert margin=5"
"<header_ weight=20"
  "<top_>"
">"
"<content_"
  "<left_><right_>"
  ">"
"<footer_ weight=20"
  "<bottom_>"
">"
;

static const char* LINKS[] = {
    "foo",
    "bar",
    "baz"
};

int main(int argc, char* argv[])
{
    nana::form fm{nana::API::make_center(1005,710)};
    
    fm.caption("Layout example");
    fm.bgcolor(nana::color_rgb(0xFFFFFF));
    
    nana::place place{fm};
    place.div(LAYOUT);
    
    nana::label
        top{fm, "top"},
        bottom{fm, "bottom"},
        left{fm, "left"},
        right{fm, "right"};
    
    top.text_align(nana::align::center);
    bottom.text_align(nana::align::center);
    
    place["left_"] << left;
    place["right_"] << right;
    
    place["top_"] << top;
    place["bottom_"] << bottom;
    
    place.collocate();
    fm.show();
    nana::exec();
    
    return 0;
}
