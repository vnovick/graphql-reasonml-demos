module CreateDraft = [%graphql
  {|
    mutation createDraft($title: String!, $author: String! $content: String!) {
      createDraft(title: $title, author: $author, content: $content) {
        id
      }
    }
  |}
];

open Utils;

module AddPostMutation = ReasonApollo.CreateMutation(CreateDraft);

type action =
  | UpdateTitle(string)
  | UpdateContent(string);

type state = {
  draftField: string,
  title: string,
};

let component = ReasonReact.reducerComponent("postsCompose");

let make = _children => {
  ...component,
  initialState: () => {draftField: "", title: ""},
  reducer: (action, state) =>
    switch (action) {
    | UpdateTitle(title) => ReasonReact.Update({...state, title})
    | UpdateContent(content) =>
      ReasonReact.Update({...state, draftField: content})
    },
  render: self => {
    <div className="card textarea_container">
      <input
        className="title"
        value={self.state.title}
        onChange={_event =>
          self.send(UpdateTitle(_event->ReactEvent.Form.target##value))
        }
      />
      <textarea
        className="area"
        value={self.state.draftField}
        onChange={_event =>
          self.send(UpdateContent(_event->ReactEvent.Form.target##value))
        }
      />
      <div className="buttonGroup">
        <AddPostMutation>
          ...{(mutation, _) => {
            let createDraft =
              CreateDraft.make(
                ~title=self.state.title,
                ~content=self.state.draftField,
                ~author="Vladimir Novick",
                (),
              );
            <button
              onClick={_mouseEvent =>
                mutation(
                  ~variables=createDraft##variables,
                  ~refetchQueries=[|"getAllPosts"|],
                  (),
                )
                |> ignore
              }>
              {"Add Draft" |> ste}
            </button>;
          }}
        </AddPostMutation>
      </div>
    </div>;
  },
};