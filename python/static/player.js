/**
 * Created by utsav on 4/7/15.
 */
function submit()
{
    var data = {};
    for(var i = 0; i < window.playernum; i++) {
        data[i] = $("#code" + i).val();
        if(data[i] == ""){
            return;
        }
    }

    $.post('/game', data).done(function(html){

        $("#gamebox").html(html);

    });

    event.preventDefault();
}