import json
from pathlib import Path
from jinja2 import Environment, FileSystemLoader, select_autoescape
import networkx as nx
import numpy as np

env = Environment(
    loader=FileSystemLoader(searchpath="./software/tools/templates/"),
    autoescape=select_autoescape(),
)


def generate_FSM(config_path: Path):
    fsm_description = read_json(config_path)
    generate_FSM_code(
        fsm_description,
        output_dir=Path("./software/macro/GccApplication5/include"),
    )
    generate_FSM_html(fsm_description, output_file="FSM.html")


def read_json(file: Path) -> dict:
    with file.open("r") as fh:
        j = json.load(fh)
    return j


def generate_FSM_code(
    fsm_description: dict,
    output_dir: Path,
):
    output_dir.mkdir(parents=True, exist_ok=True)

    states = set()
    conditions = set()
    for transition in fsm_description["transitions"]:
        conditions.add(transition["condition"])
        states.add(transition["state"])
        states.add(transition["next_state"])

    states = sorted(states)
    conditions = sorted(conditions)

    transition_table_sorted_by_state = {state: [] for state in states}
    for transition in fsm_description["transitions"]:
        transition_table_sorted_by_state[transition["state"]].append(transition)

    # State Actions
    state_actions_code_path = output_dir / "state_actions.c"
    # check which actions to be updated
    if state_actions_code_path.exists():
        with state_actions_code_path.open("r") as fh:
            code_content = "\n".join(fh.readlines())
        states = [
            state for state in states if not f"void {state}Action()" in code_content
        ]

    template = env.get_template("FSM_state_actions.c.jinja")
    with open(state_actions_code_path, "a", encoding="utf-8") as fh:
        fh.writelines(template.render(states=states))

    # Transition Conditions
    transition_conditions_code_path = output_dir / "transition_conditions.c"
    # check which conditions to be added
    if transition_conditions_code_path.exists():
        with transition_conditions_code_path.open("r") as fh:
            code_content = "\n".join(fh.readlines())
        conditions = [
            condition
            for condition in conditions
            if not f"pred_{condition}()" in code_content
        ]
    template = env.get_template("FSM_transition_conditions.c.jinja")
    with open(transition_conditions_code_path, "a", encoding="utf-8") as fh:
        fh.writelines(template.render(conditions=conditions))

    # Transition Table
    template = env.get_template("FSM_transition_table.c.jinja")
    with open(output_dir / "transition_table.c", "w", encoding="utf-8") as fh:
        fh.writelines(
            template.render(transition_table=transition_table_sorted_by_state)
        )


def get_state_html(x, y, name: str, is_intial=False):
    is_intial_str = "is-inital" if is_intial else ""
    html = f"""<g class="state">
    <circle class="state-circle {is_intial_str}" cx="{x}" cy="{y}" r="40" id="{name}"></circle>
    <text class="state-label" x="{x}" y="{y}">{name}</text>
    </g>"""
    return html


def get_transition_html(x1, y1, x2, y2, name):
    x_center = x1 * 0.4 + x2 * 0.6
    y_center = y1 * 0.4 + y2 * 0.6
    html = f"""<g class="transition">
    <line class="transition-edge" x1="{x1}" y1="{y1}" x2="{x2}" y2="{y2}"></line>
    <text class="condition-label" x="{x_center}" y="{y_center}">{name}</text>
    </g>"""
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
    state_positions = nx.planar_layout(G)

    canvas_dimensions = np.array([800, 800])
    raw_positions = np.stack(list(state_positions.values()))

    offset = raw_positions.min(axis=0) - 0.2
    scale = raw_positions.max(axis=0) - raw_positions.min(axis=0) + 0.3

    states_html = []
    for idx, (state_name, position) in enumerate(state_positions.items()):
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

    with open(output_file, "w", encoding="utf-8") as fh:
        fh.writelines(rendered_html)


if __name__ == "__main__":
    config_path = Path(r"software\tools\state_maschine.json")

    generate_FSM(config_path)
