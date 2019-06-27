@extends('layouts.app')
<style>
.political{
    font-size: 25px;
    font-weight:bold;
}
</style>
@section('content')
<div class="container">
<padding-left class="political">
    <div><a href="http://127.0.0.1:8000/home/agents">Agents</a></div>
    <div><a href="http://127.0.0.1:8000/home/members">Members</a></div>
    <div><a href="http://127.0.0.1:8000/home/funding">Funding</a></div>
 </padding-left>
</div>
@endsection
