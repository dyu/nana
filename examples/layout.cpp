#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/label.hpp>
#include <forward_list>

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
"<footer_ weight=20 margin=[0,30%]>"
;

static const char* LINKS[] = {
    "<color=0x0080FF target=\"0\">foo</>",
    "<color=0x0080FF target=\"1\">bar</>",
    "<color=0x0080FF target=\"2\">baz</>"
};

void links$$clicked(nana::label::command cmd, const std::string& target)
{
    if (nana::label::command::click == cmd)
    {
        // TODO
        //nana::msgbox mb(target);
        //mb << "the target \"" << target << "\" is clicked";
        //mb();
    }
}

int main(int argc, char* argv[])
{
    nana::form fm{nana::API::make_center(1005,710)};
    
    fm.caption("Layout example");
    fm.bgcolor(nana::color_rgb(0xFFFFFF));
    
    nana::place place{fm};
    place.div(LAYOUT);
    
    nana::label
        top{fm, "top"},
        left{fm, "left"},
        right{fm, "right"};
    
    top.text_align(nana::align::center);
    
    place["left_"] << left;
    place["right_"] << right;
    
    place["top_"] << top;
    
    std::forward_list<nana::label> links;
    for (auto text : LINKS)
    {
        links.emplace_front(fm.handle());
        auto& link = links.front();
        
        link.text_align(nana::align::center)
            .format(true)
            .add_format_listener(links$$clicked)
            .caption(text);
        
        place["footer_"] << link;
    }
    
    place.collocate();
    fm.show();
    nana::exec();
    
    return 0;
}
