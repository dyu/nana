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

struct App
{
    std::forward_list<nana::label> links;
    
    nana::form fm{ {273, 0, 1005,710} };
    nana::place place{ fm };
    nana::label bottom{ fm, "Copyright 2018 <color=0x0080FF>David Yu</>" };
    nana::label c1{ fm, "c1" };
    nana::label c2{ fm, "c2" };
    nana::label left{ fm, "left" };
    nana::label right { fm, "right" };
    
    int current_selected = 0;
    std::string current_target = "content_0";
    
    void links$$clicked(nana::label::command cmd, const std::string& target)
    {
        int selected = target.back() - 48;
        if (nana::label::command::click != cmd || selected == current_selected)
            return;
        
        // hide current
        place.field_display(current_target.c_str(), false);
        
        // set current
        current_selected = selected;
        current_target[current_target.size() - 1] = target.back();
        
        // show
        place.field_display(target.c_str(), true);
        place.collocate();
    }
    
    int show()
    {
        fm.caption("Layout example");
        fm.bgcolor(nana::color_rgb(0xFFFFFF));
        place.div(LAYOUT);
        
        place["left_"] << left;
        place["right_"] << right;
        place["content_1"] << c1;
        place["content_2"] << c2;
        
        auto listener = [this](nana::label::command cmd, const std::string& target) {
            this->links$$clicked(cmd, target);
        };
                
        for (auto text : LINKS)
        {
            links.emplace_front(fm.handle());
            auto& link = links.front();
            
            link.text_align(nana::align::center)
                .format(true)
                .add_format_listener(listener)
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
};

int main(int argc, char* argv[])
{
    App app;
    return app.show();
}
