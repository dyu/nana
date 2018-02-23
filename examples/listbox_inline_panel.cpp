#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/drawing.hpp>

struct Panel : nana::panel<true>
{
    const char* layout;
    nana::place place;
    
    Panel(const char* layout) : nana::panel<true>(), layout(layout)
    {
        
    }
    
private:
    void _m_complete_creation() override
    {
        place.bind(*this);
        place.div(layout);
        
        transparent(true);
        
        // draw a simple border.
        /*nana::drawing dw(*this);
        dw.draw([](nana::paint::graphics& graph)
        {
            graph.rectangle(false, nana::colors::gray_border);
        });*/
    }
};

//Creates a textbox and button
//textbox shows the value of the sub item
//button is used to delete the item.
class inline_widget : public nana::listbox::inline_notifier_interface
{
    inline_indicator * indicator_{ nullptr };
    index_type  pos_ ;
    Panel pnl_{
        "margin=[1,10]"
        "<lbl_ weight=60%>"
        "<txt_ weight=30% margin=[0,5]>"
        "<btn_ weight=10%>" 
    };
    nana::label lbl_;
    nana::textbox txt_;
    nana::button btn_;

private:
    //Creates inline widget
    //listbox calls this method to create the widget
    //The position and size of widget can be ignored in this process
    void create(nana::window wd) override
    {
        auto $selected = [this]() {
            indicator_->selected(pos_);
        };
        
        pnl_.create(wd);
        
        // label
        lbl_.create(pnl_);
        lbl_.transparent(true)
            .format(true)
            .events().click($selected);
        pnl_.place["lbl_"] << lbl_;
        
        // textbox
        txt_.create(pnl_);
        txt_.events().click($selected);
        pnl_.place["txt_"] << txt_;
        
        // button
        btn_.create(pnl_);
        btn_.caption("Delete");
        btn_.events().click([this]
        {
            //Delete the item when button is clicked
            auto & lsbox = dynamic_cast<nana::listbox&>(indicator_->host());
            lsbox.erase(lsbox.at(pos_));
        });
        pnl_.place["btn_"] << btn_;
    }

    //Activates the inline widget, bound to a certain item of the listbox
    //The inline_indicator is an object to operate the listbox item,
    //pos is an index object refers to the item of listbox
    void activate(inline_indicator& ind, index_type pos) override
    {
        indicator_ = &ind;
        pos_ = pos;
    }

    //Notify the item status
    //This method is introduced since 1.4.1
    void notify_status(status_type status, bool status_on) override
    {
        switch(status)
        {
        case status_type::selecting:
            //If status_on is true, the item is selecting.
            //If status on is false, the item is unselecting
            break;
        case status_type::checking:
            //If status_on is true, the item is checking
            //If status_on is false, the item is unchecking
            break;
        }
    }
    
    //Sets the inline widget size
    //dimension represents the max size can be set
    //The coordinate of inline widget is a logical coordinate to the sub item of listbox
    void resize(const nana::size& d) override
    {
        pnl_.size(d);
    }

    //Sets the value of inline widget with the value of the sub item
    void set(const std::string& value) override
    {
        lbl_.caption(value);
    }

    //Determines whether to draw the value of sub item
    //e.g, when the inline widgets covers the whole background of the sub item,
    //it should return false to avoid listbox useless drawing
    bool whether_to_draw() const override
    {
        return false;
    }
private:
};

static const int WIDTH = 1005,
        HEIGHT = 710,
        MARGIN = 5,
        // listbox
        LB_OUTER = MARGIN * 2,
        LB_HEIGHT = HEIGHT - LB_OUTER,
        LB_WIDTH = WIDTH - LB_OUTER,
        // inner
        LB_FIELDS = 1,
        LB_INNER = MARGIN * 3 * LB_FIELDS,
        FIELD_WIDTH = LB_WIDTH - LB_INNER;

int main()
{
    nana::form fm{ {10, 10, WIDTH, HEIGHT} };
    fm.bgcolor(nana::colors::white);
    
    nana::listbox lsbox{ fm, { MARGIN, MARGIN, LB_WIDTH, LB_HEIGHT } };

    //Create two columns
    lsbox.show_header(false);
    lsbox.append_header("", FIELD_WIDTH);
    //lsbox.append_header("column 1");
    
    //Set the inline_widget, the first column of category 0, the second column of category 1
    lsbox.at(0).inline_factory(0, nana::pat::make_factory<inline_widget>());
    
    //Then append items
    lsbox.at(0).append({ std::string(R"--(<color=0x0080FF size=11>foo</>)--") });
    lsbox.at(0).append({ std::string(R"--(<color=0x0080FF size=11>bar</>)--") });
    lsbox.at(0).append({ std::string(R"--(<color=0x0080FF size=11>baz</>)--") });
    
    //Create a new category
    //lsbox.append("Category 1");

    //Append items for category 1
    //lsbox.at(1).append({ std::string("Hello4"), std::string("World4") });
    //lsbox.at(1).append({ std::string("Hello5"), std::string("World5") });
    //lsbox.at(1).append({ std::string("Hello6"), std::string("World6") });
    //lsbox.at(1).append({ std::string("Hello7"), std::string("World7") });
    
    fm.show();
    nana::exec();
}
