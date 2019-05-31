
//Do everything when document is loaded
$(document).ready(function(){

    //Hide header
    $("div#header").hide();

    //Hide RGB2
    $("div#rgb2").hide();

    //Hide antiphase checkbox
    $("label.antiphase-label").hide();

    //Show/hide header on click on button
    $("button#hide-header-btn").on("click", function(){
        $("div#header").slideToggle("slow");
        $(this).find("i").toggleClass("la-angle-double-up");
        $(this).find("i").toggleClass("la-angle-double-down");
    });

    //Switch led settings
    $("div#led-switch button.btn").on("click", function(){
        let target = $(this).attr("target");
        $("div#led-switch button").each(function(){
            if (target != $(this).attr("target"))
            {
                $(this).removeClass("selected");
            }
        });
        $((target == "#rgb1" ? "#rgb2" : "#rgb1")).fadeOut("fast", function(){
            $(target).fadeIn("fast");
        });
        
        $(this).addClass("selected");
    });

    //Prepare color selections
    $("input.color-input").wheelColorPicker({
        sliders: "whsvp",
        mobile: true,
        animDuration: 200
    });

    //Change preview color
    $("input.color-input").on("colorchange", function(){
        let target = $(this).attr("target");
        $(target).css("background-color", "#" + $(this).wheelColorPicker("getValue"));
    });

    //Variable checkbox
    $("input.variable-color-check").on("change", function(){
        let target = $(this).attr("target");
        let checked = $(this).is(":checked");

        //Disable color choose
        $("input[target='" + target + "']").each(function(){
            if ($(this).hasClass("variable-color-check") == false)
            {
                if (checked)
                {
                    $(this).prop("disabled", true);
                }
                else
                {
                    $(this).prop("disabled", false);
                }
            }
        });

        //Change preview background
        if (checked)
        {
            $(target).css("background-color", "white");
            $(target).css("background-image", "linear-gradient(to right, red,orange,yellow,green,cyan,blue,magenta,red");
        }
        else
        {
            $(target).css("background-color", getPreviewColor(target));
            $(target).css("background-image", "none");
        }

        //Offer antiphase
        if (checked && $("input.same-radio").is(":checked"))
        {
            $("label.antiphase-label").fadeIn("fast");
        }
        else
        {
            $("label.antiphase-label").fadeOut("fast");
        }
    });
});

//Function to get preview color
function getPreviewColor(name)
{
    var reti = "#000000";
    $("input[target='" + name + "']").each(function(){
        if ($(this).hasClass("color-input"))
        {
            reti = "#" + $(this).wheelColorPicker("getValue");
        }
    });
    return reti;
}
