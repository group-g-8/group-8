<?php

/*
|--------------------------------------------------------------------------
| Web Routes
|--------------------------------------------------------------------------
|
| Here is where you can register web routes for your application. These
| routes are loaded by the RouteServiceProvider within a group which
| contains the "web" middleware group. Now create something great!
|
*/

Route::get('/', function () {
    return view('welcome');
});

Route::get('/home/funding', function () {
    return view('funding');
});

Route::get('/home/funding/addwellwisher', function () {
    return view('addwellwisher');
});

Route::get('/home/members', function () {
    return view('members');
});

Route::get('/home/agents', function () {
    return view('agents');
});
Auth::routes();

Route::get('/home', 'HomeController@index')->name('home');
