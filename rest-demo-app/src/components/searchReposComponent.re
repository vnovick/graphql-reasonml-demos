type repo = {
  id: int,
  node_id: string,
  name: string,
  full_name: string,
  private: bool,
  owner,
  description: string,
  html_url: string
}
and owner = {
  login: string,
  id: int,
  node_id: string,
  avatar_url: string,
  gravatar_id: string,
  url: string,
  html_url: string,
  followers_url: string,
  following_url: string,
  gists_url: string,
  starred_url: string,
  subscriptions_url: string,
  organizations_url: string,
  repos_url: string,
  events_url: string,
  received_events_url: string,
  site_admin: bool,
};

type state =
  | NotAsked
  | Loading
  | Failure
  | Success(list(repo));

module Decode = {
  let ownerDecode = json =>
    Json.Decode.{
      login: field("login", string, json),
      id: field("id", int, json),
      node_id: field("node_id", string, json),
      avatar_url: field("avatar_url", string, json),
      gravatar_id: field("gravatar_id", string, json),
      url: field("url", string, json),
      html_url: field("html_url", string, json),
      followers_url: field("followers_url", string, json),
      following_url: field("following_url", string, json),
      gists_url: field("gists_url", string, json),
      starred_url: field("starred_url", string, json),
      subscriptions_url: field("subscriptions_url", string, json),
      organizations_url: field("organizations_url", string, json),
      repos_url: field("repos_url", string, json),
      events_url: field("events_url", string, json),
      received_events_url: field("received_events_url", string, json),
      site_admin: field("site_admin", bool, json),
    };

  exception ValueNotFound(string);
  let checkForExistance = (~fieldName, value) =>
    switch (value) {
    | None =>
      Js.log(fieldName ++ " is null or does not exist");
      "";
    | Some(value) => value
    };

  let repo = json =>
    Json.Decode.{
      id: json |> field("id", int),
      node_id: json |> field("node_id", string),
      name: json |> field("name", string),
      full_name: json |> field("full_name", string),
      private: json |> field("private", bool),
      owner: json |> field("owner", ownerDecode),
      description:
        json
        |> optional(field("description", string))
        |> checkForExistance(~fieldName="description"),
      html_url: json |> field("html_url", string)
    };
  let repos = (json): list(repo) => Json.Decode.list(repo, json);
};

let accessToken: string = "";

let fetchRepos = userName =>
  Js.Promise.(
    Fetch.fetch(
      "https://api.github.com/users/"
      ++ userName
      ++ "/repos?access_token="
      ++ accessToken,
    )
    |> then_(Fetch.Response.json)
    |> then_(json =>
         json |> Decode.repos |> (repos => Some(repos) |> resolve)
       )
    |> catch(_err => resolve(None))
  );

type action =
  | LoadRepos
  | LoadedRepos(list(repo))
  | LoadReposFailed;

let component = ReasonReact.reducerComponent("FetchComponent");

let str = ReasonReact.string;
let make = (~userName, _children) => {
  ...component,
  initialState: () => NotAsked,
  reducer: (action, _state) =>
    switch (action) {
    | LoadRepos =>
      ReasonReact.UpdateWithSideEffects(
        Loading,
        self =>
          Js.Promise.(
            fetchRepos(userName)
            |> then_(result =>
                 switch (result) {
                 | Some(repos) => resolve(self.send(LoadedRepos(repos)))
                 | None => resolve(self.send(LoadReposFailed))
                 }
               )
            |> ignore
          ),
      )
    | LoadedRepos(repos) => ReasonReact.Update(Success(repos))
    | LoadReposFailed => ReasonReact.Update(Failure)
    },
  render: self =>
    switch (self.state) {
    | NotAsked =>
      <div>
        <button onClick={_event => self.send(LoadRepos)}>
          {str("Load Repos")}
        </button>
      </div>
    | Loading => <div> {str("Loading...")} </div>
    | Failure => <div> {str("Something went wrong!")} </div>
    | Success(repos) =>
      <div>
        <button onClick={_event => self.send(LoadRepos)}>
          {str("Load Repos")}
        </button>
        <ul className="reposGrid">
          {repos
           |> List.map(repo =>
                <li key={string_of_int(repo.id)} className="listItem">
                  <div className="container">
                    <img src={repo.owner.avatar_url} className="avatar" />
                    <label> {str(repo.owner.login)} </label>
                  </div>
                  <div className="container">
                    <a href={repo.html_url} target="_blank">{str(repo.name)}</a>
                    <p> {str(repo.description)} </p>
                  </div>
                </li>
              )
           /* <p> {str(repo.description)} </p> */
           |> Array.of_list
           |> ReasonReact.array}
        </ul>
      </div>
    },
};