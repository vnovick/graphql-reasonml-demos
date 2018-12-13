type comment = {
  id: string,
  content: string
}

type post = {
  id: string,
  title: string,
  content: string,
  published: bool,
  comments: option(Js.Array.t(option(comment)))
}

module PostsBoard = [%graphql
  {|
    query getAllPosts{
      posts @bsRecord {
        id,
        title,
        content,
        published,
        comments @bsRecord {
          id
          content
        }
      }
    }
  |}
];

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

module GetPostsQuery = ReasonApollo.CreateQuery(PostsBoard);

let component = ReasonReact.statelessComponent("PostsBoard");

let noPostsUI = <li> {"No Posts" |> ste} </li>;

let getCommentUI = (comment: comment) => {
  <li> { comment.content |> ste }</li>
}

let getPostUI = post =>
  <li className="film">
    <h3> {post.title |> ste} </h3>
    <div className="row">
      <label> {"Content: " |> ste} </label>
      <p> {post.content |> ste} </p>
    </div>
    <div className="row">
      <label> {"Is Published" |> ste} </label>
      <p> {post.published |> string_of_bool |> ste} </p>
    </div>
    {switch (post.comments) {
     | None => ReasonReact.null
     | Some(comments) =>
       <div>
         <ul>
           {comments
            -> Belt.Array.keepMap(comment => comment)
            |> Array.map(getCommentUI)
            |> ReasonReact.array}
         </ul>
       </div>
     }}
  </li>;

let make = _children => {
  ...component,
  render: self => {
    let starWarsFilmsQuery = PostsBoard.make();
    <GetPostsQuery>
      ...{({result}) =>
        switch (result) {
        | Loading => <div> {"Loading" |> ste} </div>
        | Error(error) => <div> {error##message |> ste} </div>
        | Data(response) =>
          <div className="board">
            <ul className="listGrid">
              {switch (response##posts) {
               | [||] => noPostsUI
               | posts =>
                 <div className="container">
                   {posts
                    |> Array.map(getPostUI)
                    |> ReasonReact.array}
                 </div>
               }}
            </ul>
          </div>
        }
      }
    </GetPostsQuery>;
  },
};