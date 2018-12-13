[%bs.raw {|require('./app.css')|}];

[@bs.module] external logo: string = "./logo.svg";
open Utils;

module Client = {
  let inMemoryCache = ApolloInMemoryCache.createInMemoryCache();
  let httpLink =
    ApolloLinks.createHttpLink(~uri="http://localhost:59149", ());

  let instance =
    ReasonApollo.createApolloClient(~link=httpLink, ~cache=inMemoryCache, ());
};

let component = ReasonReact.statelessComponent("App");

let make = (~message, _children) => {
  ...component,
  render: _self =>
    <div className="App">
      <div className="App-header">
        <img src=logo className="App-logo" alt="logo" />
        <h2> {message |> ste} </h2>
        <a href="http://localhost:57368" target="_blank">
          {"http://localhost:57368" |> ste}
        </a>
      </div>
      <ReasonApollo.Provider client=Client.instance>
        <StarWarsBoard />
      </ReasonApollo.Provider>
    </div>,
};