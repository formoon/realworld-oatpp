(window["webpackJsonp"]=window["webpackJsonp"]||[]).push([["chunk-704fe663"],{bb51:function(t,a,s){"use strict";s.r(a);var e=function(){var t=this,a=t.$createElement,s=t._self._c||a;return s("div",{staticClass:"home-page"},[t._m(0),s("div",{staticClass:"container page"},[s("div",{staticClass:"row"},[s("div",{staticClass:"col-xs-12 col-md-9"},[s("div",{staticClass:"feed-toggle"},[s("ul",{staticClass:"nav nav-pills outline-active"},[t.isAuthenticated?s("li",{staticClass:"nav-item"},[s("router-link",{staticClass:"nav-link",attrs:{to:{name:"home-my-feed"},"active-class":"active"}},[t._v("\n                Your Feed\n              ")])],1):t._e(),s("li",{staticClass:"nav-item"},[s("router-link",{staticClass:"nav-link",attrs:{to:{name:"home"},exact:"","active-class":"active"}},[t._v("\n                Global Feed\n              ")])],1),t.tag?s("li",{staticClass:"nav-item"},[s("router-link",{staticClass:"nav-link",attrs:{to:{name:"home-tag",params:{tag:t.tag}},"active-class":"active"}},[s("i",{staticClass:"ion-pound"}),t._v(" "+t._s(t.tag)+"\n              ")])],1):t._e()])]),s("router-view")],1),s("div",{staticClass:"col-xs-0 col-md-3"},[s("div",{staticClass:"sidebar"},[s("p",[t._v("Popular Tags")]),s("div",{staticClass:"tag-list"},t._l(t.tags,(function(t,a){return s("RwvTag",{key:a,attrs:{name:t}})})),1)])])])])])},i=[function(){var t=this,a=t.$createElement,s=t._self._c||a;return s("div",{staticClass:"banner"},[s("div",{staticClass:"container"},[s("h1",{staticClass:"logo-font"},[t._v("R&D communication board")]),s("p",[t._v("Share your idea, Share your work.")])])])}],n=s("2f62"),l=s("f53f"),c=s("6c33"),o={name:"home",components:{RwvTag:l["a"]},mounted(){this.$store.dispatch(c["r"])},computed:{...Object(n["b"])(["isAuthenticated","tags"]),tag(){return this.$route.params.tag}}},r=o,u=s("2877"),m=Object(u["a"])(r,e,i,!1,null,null,null);a["default"]=m.exports},f53f:function(t,a,s){"use strict";var e=function(){var t=this,a=t.$createElement,s=t._self._c||a;return s("router-link",{class:t.className,attrs:{to:t.homeRoute},domProps:{textContent:t._s(t.name)}})},i=[],n={name:"RwvTag",props:{name:{type:String,required:!0},className:{type:String,default:"tag-pill tag-default"}},computed:{homeRoute:function(){return{name:"home-tag",params:{tag:this.name}}}}},l=n,c=s("2877"),o=Object(c["a"])(l,e,i,!1,null,null,null);a["a"]=o.exports}}]);
//# sourceMappingURL=chunk-704fe663.fb092546.js.map