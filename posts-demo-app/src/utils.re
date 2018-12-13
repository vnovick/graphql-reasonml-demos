let ste = ReasonReact.string;

let nullableStringToElement = obj =>
  (
    switch (obj) {
    | None => "No data"
    | Some(str) => str
    }
  )
  |> ste;