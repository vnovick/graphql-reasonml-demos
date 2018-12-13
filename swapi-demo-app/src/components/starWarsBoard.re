type film = {
  title: option(string),
  director: option(string),
  producers: option(Js.Array.t(option(string))),
  releaseDate: option(string),
};

module StarWarsBoard = [%graphql
  {|
           query getFilms($number: Int) {
           allFilms(first: $number) {
             films @bsRecord {
               title
               director,
               producers,
               releaseDate,
             }
           }
         }
         |}
];

open Utils;

module GetStarWarsFilmsQuery = ReasonApollo.CreateQuery(StarWarsBoard);

let component = ReasonReact.statelessComponent("StarWarsBoard");

let noFilmsUI = <li> {"No Films" |> ste} </li>;

let getFilmUI = film =>
  <li className="film">
    <h3> {film.title |> nullableStringToElement} </h3>
    <div className="row">
      <label> {"Director: " |> ste} </label>
      <p> {film.director |> nullableStringToElement} </p>
    </div>
    <div className="row">
      <label> {"Release date:" |> ste} </label>
      <p> {film.releaseDate |> nullableStringToElement} </p>
    </div>
    {switch (film.producers) {
     | None => ReasonReact.null
     | Some(producers) =>
       <div>
         <label> {"Producers" |> ste} </label>
         <ul>
           {producers
            |> Array.map(producer =>
                 <li> {producer |> nullableStringToElement} </li>
               )
            |> ReasonReact.array}
         </ul>
       </div>
     }}
  </li>;

let make = _children => {
  ...component,
  render: self => {
    let starWarsFilmsQuery = StarWarsBoard.make(~number=10, ());
    <GetStarWarsFilmsQuery variables=starWarsFilmsQuery##variables>
      ...{({result}) =>
        switch (result) {
        | Loading => <div> {"Loading" |> ste} </div>
        | Error(error) => <div> {error##message |> ste} </div>
        | Data(response) =>
          let films = [%get_in response##allFilms#??films];
          <div className="board">
            <ul className="listGrid">
              {switch (films) {
               | None => noFilmsUI
               | Some(films) =>
                 <div className="container">
                   {films->Belt.Array.keepMap(film => film)
                    |> Array.map(getFilmUI)
                    |> ReasonReact.array}
                 </div>
               }}
            </ul>
          </div>;
        }
      }
    </GetStarWarsFilmsQuery>;
  },
};

/* let films = [%get_in response##allFilms#??films]; */