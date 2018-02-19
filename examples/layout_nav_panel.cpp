#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/panel.hpp>
#include <forward_list>

namespace ui {
    
    struct Form : nana::form
    {
        Form(nana::rectangle rect, const char* title, unsigned bg) : nana::form(rect)
        {
            caption(title);
            bgcolor(nana::color_rgb(bg));
        }
    };
    
    struct Panel : nana::panel<true>
    {
        nana::place place{ *this };
        
        Panel(nana::widget& owner, const char* layout) : nana::panel<true>(owner)
        {
            place.div(layout);
        }
    };
    
} // ui

struct Content0 : ui::Panel
{
    nana::label left{ *this, "left" };
    nana::label right{ *this, "right" };
    
    Content0(nana::widget& owner) : Panel(owner, 
        "<left_ weight=30%>"
        "<right_>"
    )
    {
        left.bgcolor(nana::color_rgb(0xFAFAFA));
        
        place["left_"] << left;
        place["right_"] << right;
        place.collocate();
    }
};

static const char* LINKS[] = {
    "<color=0x0080FF target=\"content_0\">Home</>",
    "<color=0x0080FF target=\"content_1\">Test</>",
    "<color=0x0080FF target=\"content_2\">About</>"
};

struct App
{
    std::forward_list<nana::label> links;
    int current_selected{ 0 };
    std::string current_target{ "content_0" };
    
    ui::Form fm{ {273, 0, 1005, 710}, "Layout example", 0xFFFFFF };
    nana::place place{ fm };
    nana::label bottom{ fm, "Copyright 2018 <color=0x0080FF>David Yu</>" };
    ui::Panel content{ fm,
        "vert"
        "<content_0>"
        "<content_1>"
        "<content_2>"
    };
    Content0 c0{ content };
    nana::label c1{ content, "c1" };
    nana::label c2{ content, "c2" };
    
    App()
    {
        place.div(
            "vert margin=5"
            "<header_ weight=20 margin=[0,30%]>"
            "<content_>"
            "<footer_ weight=20>"
        );
        
        c2.bgcolor(nana::color_rgb(0xFCFCFC));
        
        // content
        content.place["content_0"] << c0;
        
        content.place["content_1"] << c1;
        content.place.field_display("content_1", false);
        
        content.place["content_2"] << c2;
        content.place.field_display("content_2", false);
        
        content.place.collocate();
        place["content_"] << content;
        
        // bottom
        place["footer_"] << bottom.text_align(nana::align::center).format(true);
    }
    
    void links$$(nana::label::command cmd, const std::string& target)
    {
        int selected = target.back() - 48;
        if (nana::label::command::click != cmd || selected == current_selected)
            return;
        
        // hide current
        content.place.field_display(current_target.c_str(), false);
        
        // set current
        current_selected = selected;
        current_target[current_target.size() - 1] = target.back();
        
        // show
        content.place.field_display(target.c_str(), true);
        content.place.collocate();
    }
    
    int show()
    {
        // header
        auto listener = [this](nana::label::command cmd, const std::string& target) {
            this->links$$(cmd, target);
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
