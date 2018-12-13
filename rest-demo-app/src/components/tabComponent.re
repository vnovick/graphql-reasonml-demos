type state =
 | Tab1
 | Tab2

type action =
 | SwitchTab(state)

let component = ReasonReact.reducerComponent("TabComponent");
let make = (_children) => {
  ...component,
  initialState: () => Tab1,
  reducer: (action, _state) =>
    switch(action) {
      | SwitchTab(Tab1) => 
        ReasonReact.Update(Tab1)
      | SwitchTab(Tab2) => ReasonReact.Update(Tab2)
    },
  render: self => 
    <div>
      <ul className="menu">
        <li className="card" onClick={_event => self.send(SwitchTab(Tab1))}>{"Load Dummy Posts" |> ReasonReact.string }</li>
        <li className="card" onClick={_event => self.send(SwitchTab(Tab2))}>{"Search Github Repos" |> ReasonReact.string }</li>
      </ul>
      <div>
        {
          switch (self.state) {
          | Tab1 => (
            <div className="tab1">
              <DummyPostsComponent />
            </div>
          )
          | Tab2 => (
            <div className="tab2">
              <ReposForm />
            </div>
          )
        }
      }
      </div>
    </div>
}
    