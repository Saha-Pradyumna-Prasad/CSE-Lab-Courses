document.getElementById('paymentForm').addEventListener('submit', function(event) {
    event.preventDefault(); 
    const username = document.getElementById('username').value;
    const email = document.getElementById('email').value;
    const phone = document.getElementById('phone').value;
    const address = document.getElementById('address').value;
    const country = document.getElementById('country').value;
    const amount = document.getElementById('amount').value;
    const payment = document.getElementById('payment').value;

    const queryString = `?username=${encodeURIComponent(username)}&email=${encodeURIComponent(email)}&phone=${encodeURIComponent(phone)}&address=${encodeURIComponent(address)}&country=${encodeURIComponent(country)}&amount=${encodeURIComponent(amount)}&payment=${encodeURIComponent(payment)}`;
    window.location.href = 'after click submit btn.html' + queryString;
});

