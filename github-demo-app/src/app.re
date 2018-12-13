[%bs.raw {|require('./app.css')|}];

[@bs.module] external logo: string = "./logo.svg";
open Utils;

module Client = {
  let inMemoryCache = ApolloInMemoryCache.createInMemoryCache();

  let accessToken = "";

  let contextHandler = () => {
    let headers = {
      "headers": {
        "authorization": {j|Bearer $accessToken|j},
      },
    };
    headers;
  };

  let authLink = ApolloLinks.createContextLink(contextHandler);

  let httpLink =
    ApolloLinks.createHttpLink(~uri="https://api.github.com/graphql", ());

  let instance =
    ReasonApollo.createApolloClient(
      ~link=ApolloLinks.from([|authLink, httpLink|]),
      ~cache=inMemoryCache,
      (),
    );
};


let component = ReasonReact.statelessComponent("App");

let make = (~message, _children) => {
  ...component,
  render: _self =>
    <div className="App">
      <div className="App-header">
        <img src=logo className="App-logo" alt="logo" />
        <h2> {message |> ste} </h2>
        <a href="https://api.github.com/graphql" target="_blank">
          {"http://https://api.github.com/graphql" |> ste}
        </a>
      </div>
      <ReasonApollo.Provider client=Client.instance>
        <GithubBoard />
      </ReasonApollo.Provider>
    </div>,
};