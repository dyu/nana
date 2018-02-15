#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/label.hpp>
#include <forward_list>

static const char LAYOUT[] =
//"<main_ margin=2 grid=[2,1] gap=2>"
//"<left_>|70%<right_>"
//"<left_><right_ weight=70%>"
"vert margin=5"
"<header_ weight=20 margin=[0,30%]>"
"<content_0"
  "<left_><right_>"
">"
"<content_1>"
"<content_2>"
"<footer_ weight=20>"
;

static const char* LINKS[] = {
    "<color=0x0080FF target=\"content_0\">Home</>",
    "<color=0x0080FF target=\"content_1\">Test</>",
    "<color=0x0080FF target=\"content_2\">About</>"
};

static nana::place* current_place;
static int current_selected = 0;
static std::string current_target = "content_0";

void links$$clicked(nana::label::command cmd, const std::string& target)
{
    int selected = target.back() - 40;
    if (nana::label::command::click != cmd || selected == current_selected)
        return;
    
    // hide current
    current_place->field_display(current_target.c_str(), false);
    
    // set current
    current_selected = selected;
    current_target[current_target.size() - 1] = target.back();
    
    // show
    current_place->field_display(target.c_str(), true);
}

int main(int argc, char* argv[])
{
    nana::form fm{nana::API::make_center(1005,710)};
    
    fm.caption("Layout example");
    fm.bgcolor(nana::color_rgb(0xFFFFFF));
    
    nana::place place{fm};
    place.div(LAYOUT);
    
    current_place = &place;
    
    nana::label
        bottom{fm, "Copyright 2018 <color=0x0080FF>David Yu</>"},
        c1{fm, "c1"},
        c2{fm, "c2"},
        left{fm, "left"},
        right{fm, "right"};
    
    place["left_"] << left;
    place["right_"] << right;
    place["content_1"] << c1;
    place["content_2"] << c2;
    
    std::forward_list<nana::label> links;
    for (auto text : LINKS)
    {
        links.emplace_front(fm.handle());
        auto& link = links.front();
        
        link.text_align(nana::align::center)
            .format(true)
            .add_format_listener(links$$clicked)
            .caption(text);
        
        place["header_"] << link;
    }
    
    bottom.text_align(nana::align::center)
        .format(true);
    place["footer_"] << bottom;
    
    place.field_display("content_1", false);
    place.field_display("content_2", false);
    place.collocate();
    fm.show();
    nana::exec();
    
    return 0;
}
