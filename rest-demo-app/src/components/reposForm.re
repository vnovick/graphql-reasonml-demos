type state = {
  userName: string
};

type action =
  | Update_Input(string);

let component = ReasonReact.reducerComponent("ReposForm");

let make = _children => {
  ...component,
  initialState: () => {userName: "vnovick"},
  reducer: (action, state) =>
    switch (action) {
    | Update_Input(value) => ReasonReact.Update({ userName: value })
    },
  render: _self =>
    <div>
      <input
        onChange={
          _event =>
            _self.send(Update_Input(_event->ReactEvent.Form.target##value))
        }
      />
      <SearchReposComponent userName={_self.state.userName} />
    </div>,
};