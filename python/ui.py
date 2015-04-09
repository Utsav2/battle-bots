from flask import Flask, render_template, request, abort
import libTurnBased as lib
import random
app = Flask(__name__)

@app.route("/")
def main_page():
    return render_template('how_many_players.html')

@app.route("/rock_paper_scissors", methods=["POST"])
def rps():
    try:
        number_of_players = int(request.form["number"])
    except Exception:
        abort(400)
        return

    return render_template('rps_ui.html', n=number_of_players)


def sanitize(string):
    return string


def evaluate(code, player):
    safe_dict = dict()
    safe_dict['player'] = player
    safe_dict['random'] = random

    try:
        a = compile(sanitize(code), '<string>', 'exec')
        eval(a, {"__builtins__": None}, safe_dict)
    except SyntaxError:
        return 'Syntax error!', 200
    except NameError:
        return 'Name error!', 200

@app.route("/game", methods=["POST"])
def game():

    def play_game(gc, post_data):
            for turn in range(10):
                for i, player in enumerate(gc.get_player_list()):
                    if '__' in post_data[str(i)] or 'eval' in post_data[str(i)]:
                        return str.format('we do not trust your code, player {0}', i+1), 200
                    evaluate(post_data[str(i)], player)

            return render_template('rps.html', states=gamecore.get_state())

    data = request.form
    gamecore = lib.TB_core(len(data))

    return play_game(gamecore, data)

if __name__ == "__main__":
    app.run(debug=True)