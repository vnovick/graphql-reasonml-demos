[%bs.raw {|require('./index.css')|}];

[@bs.module "./serviceWorker"]
external register_service_worker: unit => unit = "register";
[@bs.module "./serviceWorker"]
external unregister_service_worker: unit => unit = "unregister";

ReactDOMRe.renderToElementWithId(<App message="REST example" />, "root");

unregister_service_worker();