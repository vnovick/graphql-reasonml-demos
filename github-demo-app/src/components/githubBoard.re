module GithubBoard = [%graphql
  {|
        query getUserRepos($userName: String!){
        user(login: $userName){
          avatarUrl,
          name,
          repositories(first: 100) {
            nodes {
              name,
              description,
            }
          }
        }
      }
      |}
];

open Utils;

module GetReposQuery = ReasonApollo.CreateQuery(GithubBoard);

let component = ReasonReact.statelessComponent("GithubBoard");

let make = _children => {
  ...component,
  render: self => {
    let repositoriesQuery = GithubBoard.make(~userName="vnovick", ());
    <GetReposQuery variables=repositoriesQuery##variables>
      ...{({result}) =>
        switch (result) {
        | Loading => <div> {"Loading" |> ste} </div>
        | Error(error) => <div> {error##message |> ste} </div>
        | Data(response) =>
          switch (response##user) {
          | None => <div />
          | Some(user) =>
            <div> <div> {user##name |> nullableStringToElement} </div> </div>
          }
        }
      }
    </GetReposQuery>;
  },
};