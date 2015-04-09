/**
 * Created by utsav on 4/8/15.
 */

images = {

    0: "http://4.bp.blogspot.com/_VQpcTr88tlA/S-VvIwQdr_I/AAAAAAAAAAs/UAwYFK3Sl4w/s1600/stone_01.png",
    1: "http://www.backgroundsy.com/file/large/crumpled-paper.jpg",
    2: "http://upload.wikimedia.org/wikipedia/commons/1/18/Skalm_2.JPG"

}


function play()
{

    if(window.current_state >= window.states.length)
    {
        $("#play").html("Game over!");
    }

    var state = window.states[window.current_state];

    for(var i = 0; i < state.length; i++)
    {
        $("#img"+i).attr("src", images[state[i]]);
    }

    window.current_state++;
}