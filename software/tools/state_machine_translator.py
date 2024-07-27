import json
from pathlib import Path
from jinja2 import Environment, FileSystemLoader, select_autoescape
import networkx as nx
import numpy as np


def generate_FSM(config_path: Path):
    fsm_description = read_json(config_path)
    generate_FSM_code(fsm_description, output_dir=Path("macro/generated/"))
    generate_FSM_html(fsm_description, output_file="FSM.html")


def read_json(file: Path) -> dict:
    with file.open("r") as fh:
        j = json.load(fh)
    return j


def generate_FSM_code(fsm_description: dict, output_dir: Path):
    pass


def get_state_html(x, y, name):
    html = f"""<circle class="state" cx="{x}" cy="{y}" r="40" id="{name}"></circle>
    <text class="state-label" x="{x}" y="{y}">{name}</text>
    """
    return html


def get_transition_html(x1, y1, x2, y2, name):
    x_center = (x1 + x2) / 2
    y_center = (y1 + y2) / 2
    html = f"""<line class="transition" x1="{x1}" y1="{y1}" x2="{x2}" y2="{y2}"></line>
    <text class="condition-label" x="{x_center}" y="{y_center}">{name}</text>
    """
    return html


def scale_position(position, offset, scale):
    return (position - offset) * scale


def generate_FSM_html(fsm_description: dict, output_file: Path):
    edges = set()
    states = set()
    for transition in fsm_description["transitions"]:
        state_current = transition["state"]
        state_next = transition["next_state"]
        edges.add((state_current, state_next))
        states.add(state_current)
        states.add(state_next)

    G = nx.DiGraph()

    G.add_edges_from(edges)
    state_positions = nx.spring_layout(G)

    env = Environment(
        loader=FileSystemLoader(searchpath="./software/tools/templates/"),
        autoescape=select_autoescape(),
    )

    canvas_dimensions = np.array([600, 400])
    raw_positions = np.stack(list(state_positions.values()))

    offset = raw_positions.min(axis=0)
    scale = raw_positions.max(axis=0) - raw_positions.min(axis=0)

    states_html = []
    for state_name, position in state_positions.items():
        html_canvas_position = scale_position(
            position=position,
            offset=offset,
            scale=1 / scale * canvas_dimensions,
        )
        states_html.append(
            get_state_html(html_canvas_position[0], html_canvas_position[1], state_name)
        )

    transition_html = []
    for transition in fsm_description["transitions"]:
        condition = transition["condition"]
        current_state = transition["state"]
        next_state = transition["next_state"]

        position_current_state = state_positions[current_state]
        position_current_state_scaled = scale_position(
            position=position_current_state,
            offset=offset,
            scale=1 / scale * canvas_dimensions,
        )
        position_current_next = state_positions[next_state]
        position_current_state_next = scale_position(
            position=position_current_next,
            offset=offset,
            scale=1 / scale * canvas_dimensions,
        )
        transition_html.append(
            get_transition_html(
                position_current_state_scaled[0],
                position_current_state_scaled[1],
                position_current_state_next[0],
                position_current_state_next[1],
                condition,
            )
        )

    template = env.get_template("index.html.jinja")
    rendered_html = template.render(
        {
            "size": canvas_dimensions,
            "states": states_html,
            "transitions": transition_html,
        }
    )

    with open(output_file, "w") as fh:
        fh.writelines(rendered_html)


if __name__ == "__main__":
    config_path = Path(r"software\tools\state_maschine.json")

    generate_FSM(config_path)
