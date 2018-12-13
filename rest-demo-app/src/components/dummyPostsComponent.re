type post = {
  userId: int,
  title: string,
  body: string,
  test: string,
};

type state =
  | NotAsked
  | Loading
  | Failure
  | Success(post);

module Decode = {
  let post = post =>
    Json.Decode.{
      userId: field("userId", int, post),
      title: field("title", string, post),
      body: field("body", string, post),
      test:
        switch (post |> optional(field("test", string))) {
        | None => ""
        | Some(test) => test
        },
    };
};

open Utils;

let url = "https://jsonplaceholder.typicode.com/posts/";

let fetchPosts = postId =>
  Js.Promise.(
    Fetch.fetch(url ++ string_of_int(postId))
    |> then_(Fetch.Response.json)
    |> then_(json => json |> Decode.post |> (post => Some(post) |> resolve))
    |> catch(_err => resolve(None))
  );

type action =
  | LoadPosts(int)
  | LoadedPosts(post)
  | LoadPostsFailed;

let component = ReasonReact.reducerComponent("dummyPostsComponent");

let make = _children => {
  ...component,
  initialState: () => NotAsked,
  reducer: (action, _state) =>
    switch (action) {
    | LoadPosts(postId) =>
      ReasonReact.UpdateWithSideEffects(
        Loading,
        self =>
          Js.Promise.(
            fetchPosts(postId)
            |> then_(result =>
                 switch (result) {
                 | Some(user) => resolve(self.send(LoadedPosts(user)))
                 | None => resolve(self.send(LoadPostsFailed))
                 }
               )
            |> ignore
          ),
      )
    | LoadedPosts(user) => ReasonReact.Update(Success(user))
    | LoadPostsFailed => ReasonReact.Update(Failure)
    },
  render: self =>
    switch (self.state) {
    | NotAsked =>
      <div>
        {"Click to start load Posts" |> ste}
        <button onClick={_event => self.send(LoadPosts(1))}>
          {"Load Posts" |> ste}
        </button>
      </div>
    | Loading => <div> {"Loading..." |> ste} </div>
    | Failure => <div> {"Something went wrong!" |> ste} </div>
    | Success(post) =>
      <div className="card">
        <h2> {"Post" |> ste} </h2>
        <div> {post.title |> ste} </div>
        <div> {post.body |> ste} </div>
        <div> {post.test |> ste} </div>
      </div>
    },
};